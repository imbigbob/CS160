#include "TransactionState.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../ManagementAddState/ManagementAddState.hpp" 

// Layout Constants
const float TABLE_X = 100.f; 
const float TABLE_Y = 150.f;  
const float HEADER_HEIGHT = 40.f;
const float ROW_HEIGHT = 40.f;

TransactionState::TransactionState(StateStack& stack, Context context)
    : State(stack, context), mScrollY(0.f), mTotalContentHeight(0.f) {
    
    sf::Vector2f windowSize(context.window->getSize());
    sf::Texture& backgroundTexture =
        context.textureHolder->get(Textures::ID::MenuBackground);

    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y
    );

    // --- BUTTONS ---
    auto backButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Back"
    );
    backButton->setPosition(250.f, 50.f);
    backButton->setCallback([this]() { requestStackPop(); });

    auto addButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Add"
    );
    addButton->setPosition(400.f, 50.f);
    
    // [FIX] Pass the correct mode (Income/Expense) to the Add Screen
    addButton->setCallback([this]() {
        // 0 = Income, 1 = Expense
        int modeToSend = (mMode == Mode::Income) ? 0 : 1;

        // We pass nullptr for the TypeManager because we are using the new Income/ExpenseManagers
        ManagementAddState::setPayload(modeToSend, nullptr); 

        requestStackPush(States::ID::ManagementAdd); 
    });

    auto toggleButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Show Expense"
    );
    toggleButton->setPosition(550.f, 50.f);
    
    // Set initial text
    if (mMode == Mode::Income) toggleButton->setText("Show Expense");
    else toggleButton->setText("Show Income");

    toggleButton->setCallback([this, toggleButton]() {
        if (mMode == Mode::Income) {
            mMode = Mode::Expense;
            toggleButton->setText("Show Income");
        } else {
            mMode = Mode::Income;
            toggleButton->setText("Show Expense");
        }
        reloadTable();
    });

    mGUIContainer.addComponent(backButton);
    mGUIContainer.addComponent(addButton);
    mGUIContainer.addComponent(toggleButton);

    // --- HEADER SETUP ---
    float colWidth[] = {120.f, 150.f, 100.f, 150.f, 200.f};

    mTableHeader.setSize(
        sf::Vector2f(colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3] + colWidth[4], HEADER_HEIGHT)
    );
    mTableHeader.setPosition(TABLE_X, TABLE_Y);
    mTableHeader.setFillColor(sf::Color(230, 230, 230));

    sf::Font& font = context.fontHolder->get(Fonts::ID::Dosis);
    std::vector<std::string> headers = {"Date", "Source", "Amount", "Wallet", "Description"};
    
    float currentX = TABLE_X;
    for (int i = 0; i < headers.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(headers[i]);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(currentX + 10.f, TABLE_Y + 8.f);
        mHeaderTexts.push_back(text);
        currentX += colWidth[i];
    }

    // --- VIEW SETUP ---
    float listTopY = TABLE_Y + HEADER_HEIGHT;
    mTableBounds = sf::FloatRect(0.f, listTopY, windowSize.x, windowSize.y - listTopY - 50.f);

    mTableView.setSize(mTableBounds.width, mTableBounds.height);
    mTableView.setCenter(mTableBounds.width / 2.f, listTopY + mTableBounds.height / 2.f);

    float viewportY = listTopY / windowSize.y;
    float viewportHeight = (windowSize.y - listTopY - 50.f) / windowSize.y;
    mTableView.setViewport(sf::FloatRect(0.f, viewportY, 1.f, viewportHeight));

    reloadTable();
}

void TransactionState::reloadTable() {
    mRowRects.clear();
    mRowTexts.clear();
    mScrollY = 0.f;
    updateScrollView();

    float colWidth[] = {120.f, 150.f, 100.f, 150.f, 200.f};
    float tableX = TABLE_X;
    float currentY = TABLE_Y + HEADER_HEIGHT;

    sf::Font& font = getContext().fontHolder->get(Fonts::ID::Dosis);

    // --- DATA FETCHING ---
    struct DisplayRow {
        std::string date, name, amount, wallet, desc;
    };
    std::vector<DisplayRow> rows;

    if (mMode == Mode::Income) {
        // Income Manager
        IncomeManager* mgr = getContext().incomeManager;
        if (mgr) {
            auto& list = mgr->getAll(); 
            for (int i = 0; i < list.getSize(); i++) {
                const auto& r = list[i];
                std::stringstream ss;
                ss << "$" << (int)r.getAmount();
                rows.push_back(DisplayRow{r.getDate(), r.getName(), ss.str(), r.getWalletName(), r.getDescription()});
            }
        }
    } else {
        // Expense Manager
        ExpenseManager* mgr = getContext().expenseManager;
        if (mgr) {
            auto& list = mgr->getAll();
            for (int i = 0; i < list.getSize(); i++) {
                const auto& r = list[i];
                std::stringstream ss;
                ss << "$" << (int)r.getAmount();
                rows.push_back(DisplayRow{r.getDate(), r.getName(), ss.str(), r.getWalletName(), r.getDescription()});
            }
        }
    }

    // --- DRAW ROWS ---
    for (const auto& rowData : rows) {
        sf::RectangleShape rowRect;
        float totalWidth = colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3] + colWidth[4];
        rowRect.setSize(sf::Vector2f(totalWidth, ROW_HEIGHT));
        rowRect.setPosition(tableX, currentY);
        rowRect.setFillColor(sf::Color(245, 245, 245));
        mRowRects.push_back(rowRect);

        std::vector<std::string> items = {rowData.date, rowData.name, rowData.amount, rowData.wallet, rowData.desc};

        float cx = tableX;
        for (int col = 0; col < items.size(); col++) {
            sf::Text t;
            t.setFont(font);
            t.setString(items[col]);
            t.setCharacterSize(18);
            t.setFillColor(sf::Color::Black);
            t.setPosition(cx + 10.f, currentY + 8.f);
            mRowTexts.push_back(t);
            cx += colWidth[col];
        }
        currentY += ROW_HEIGHT;
    }

    mTotalContentHeight = rows.size() * ROW_HEIGHT;
}

bool TransactionState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);

    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            mScrollY -= event.mouseWheelScroll.delta * 30.f;
            updateScrollView();
        }
    }
    return false;
}

void TransactionState::updateScrollView() {
    float visibleHeight = mTableBounds.height;
    if (mScrollY < 0.f) mScrollY = 0.f;

    float maxScroll = std::max(0.f, mTotalContentHeight - visibleHeight);
    if (mScrollY > maxScroll) mScrollY = maxScroll;

    float listTopY = TABLE_Y + HEADER_HEIGHT;
    float defaultCenterY = listTopY + visibleHeight / 2.f;

    mTableView.setCenter(mTableBounds.width / 2.f, defaultCenterY + mScrollY);
}

bool TransactionState::update(sf::Time deltaTime) { 
    reloadTable();
    return true; 
}

void TransactionState::draw() {
    sf::RenderWindow& window = *getContext().window;

    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);

    window.draw(mTableHeader);
    for (auto& t : mHeaderTexts) window.draw(t);

    window.setView(mTableView);
    for (auto& r : mRowRects) window.draw(r);
    for (auto& t : mRowTexts) window.draw(t);

    window.setView(window.getDefaultView());
    window.draw(mGUIContainer);
}