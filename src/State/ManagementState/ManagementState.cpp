#include "ManagementState.hpp"
#include "../ManagementEditState/ManagementEditState.hpp"
#include "../ManagementAddState/ManagementAddState.hpp"
#include <algorithm> 
#include <iostream>
#include <iomanip> 
#include <sstream>

// [CORRECTED PATH]
#include "../../model/Income/Income.hpp"

// Layout Constants
const float TABLE_X = 200.f;
const float TABLE_Y = 150.f; 
const float HEADER_HEIGHT = 40.f;
const float ROW_HEIGHT = 60.f;

ManagementState::ManagementState(StateStack &stack, Context context)
    : State(stack, context),
      currentMode(Mode::Income),
      mScrollY(0.f),
      mTotalContentHeight(0.f)
{
    sf::Vector2f windowSize(context.window->getSize());
    sf::Texture &backgroundTexture =
        context.textureHolder->get(Textures::ID::ManagementBackground);

    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y);

    // --- BUTTONS ---
    auto backButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Back");
    backButton->setPosition(250.f, 50.f);
    backButton->setCallback([this]()
                            { requestStackPop(); });

    modeSwitchButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Income");
    modeSwitchButton->setPosition(450.f, 50.f);
    modeSwitchButton->setCallback([this]()
    {
        // Toggle Mode
        if (currentMode == Mode::Income)
            currentMode = Mode::Expense;
        else if (currentMode == Mode::Expense)
            currentMode = Mode::Wallet;
        else
            currentMode = Mode::Income;

        // Update Button Text
        if (currentMode == Mode::Income)
            modeSwitchButton->setText("Income");
        else if (currentMode == Mode::Expense)
            modeSwitchButton->setText("Expense");
        else
            modeSwitchButton->setText("Wallet");

        reloadTable(); 
    });

    addButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Add");
    addButton->setPosition(700.f, 50.f);
    addButton->setCallback([this]() { handleAdd(); });

    mGUIContainer.addComponent(backButton);
    mGUIContainer.addComponent(modeSwitchButton);
    mGUIContainer.addComponent(addButton);

    // --- TABLE HEADER ---
    float colWidth[] = { 80.f, 300.f, 200.f };

    mTableHeader.setSize(
        sf::Vector2f(colWidth[0] + colWidth[1] + colWidth[2], HEADER_HEIGHT));
    mTableHeader.setPosition(TABLE_X, TABLE_Y);
    mTableHeader.setFillColor(sf::Color(230, 230, 230));

    sf::Font &font = context.fontHolder->get(Fonts::ID::Dosis);
    std::vector<std::string> headers = {"No.", "Details", "Action"};

    float currentX = TABLE_X;
    for (int i = 0; i < headers.size(); i++)
    {
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
    float bottomMargin = 50.f;
    float visibleHeight = windowSize.y - listTopY - bottomMargin;

    mTableBounds = sf::FloatRect(0.f, listTopY, windowSize.x, visibleHeight);

    mTableView.setSize(mTableBounds.width, mTableBounds.height);
    mTableView.setCenter(
        mTableBounds.width / 2.f, listTopY + mTableBounds.height / 2.f);

    float viewportY = listTopY / windowSize.y;
    float viewportHeight = visibleHeight / windowSize.y;
    mTableView.setViewport(sf::FloatRect(0.f, viewportY, 1.f, viewportHeight));

    reloadTable();
}

void ManagementState::handleAdd()
{
    // The ManagementAddState will access IncomeManager via Context,
    // so we don't strictly need to pass payload for Income mode anymore,
    // but we keep it for Expense/Wallet legacy support.
    ManagementAddState::setPayload(static_cast<int>(currentMode),
                                   (currentMode == Mode::Income)
                                       ? &incomeTypeManager
                                   : (currentMode == Mode::Expense)
                                       ? &expenseTypeManager
                                       : &walletTypeManager);
    requestStackPush(States::ID::ManagementAdd);
}

void ManagementState::reloadTable()
{
    // Clear UI
    mRowRects.clear();
    mRowTexts.clear();
    mEditSprites.clear();
    mDeleteSprites.clear();

    mScrollY = 0.f;
    updateScrollView();

    float tableX = TABLE_X;
    float colWidth[] = {80.f, 300.f, 200.f};
    float currentY = TABLE_Y + HEADER_HEIGHT;

    sf::Font &font = getContext().fontHolder->get(Fonts::ID::Dosis);
    const sf::Texture &editTexture = getContext().textureHolder->get(Textures::ID::Edit);
    const sf::Texture &deleteTexture = getContext().textureHolder->get(Textures::ID::Bin);

    struct DisplayRow {
        std::string col1;
        std::string col2;
    };
    std::vector<DisplayRow> displayRows;

    // --- DATA FETCHING ---
    if (currentMode == Mode::Income)
    {
        // === NEW WAY: IncomeManager ===
        IncomeManager* mgr = getContext().incomeManager;
        
        // Use the getAll() method from IncomeManager.cpp
        DynamicArray<Income>& list = mgr->getAll(); 

        for (int i = 0; i < list.getSize(); ++i)
        {
            Income& inc = list[i];
            
            std::stringstream ss;
            ss << inc.getName() << " ($" << (int)inc.getAmount() << ")";
            
            displayRows.push_back({ std::to_string(i + 1), ss.str() });
        }
    }
    else
    {
        // === OLD WAY: TypeManager ===
        TypeManager *manager = (currentMode == Mode::Expense) ? &expenseTypeManager : &walletTypeManager;
        auto &types = manager->getAllTypes();
        for (int i = 0; i < types.getSize(); ++i)
        {
            displayRows.push_back({ std::to_string(i + 1), types[i].getName() });
        }
    }

    // --- DRAW ROWS ---
    for (int i = 0; i < displayRows.size(); i++)
    {
        // Background
        sf::RectangleShape rowRect;
        rowRect.setSize(sf::Vector2f(colWidth[0] + colWidth[1] + colWidth[2], ROW_HEIGHT));
        rowRect.setPosition(tableX, currentY);
        rowRect.setFillColor((i % 2 == 0) ? sf::Color(250, 250, 250) : sf::Color(240, 240, 240));
        mRowRects.push_back(rowRect);

        // Text
        std::vector<std::string> rowData = { displayRows[i].col1, displayRows[i].col2 };

        float cx = tableX;
        for (int col = 0; col < rowData.size(); col++)
        {
            sf::Text t;
            t.setFont(font);
            t.setString(rowData[col]);
            t.setCharacterSize(25);
            t.setFillColor(sf::Color::Black);
            t.setPosition(cx + 10.f, currentY + 8.f);
            mRowTexts.push_back(t);
            cx += colWidth[col];
        }

        // Icons
        sf::Sprite editSprite(editTexture);
        editSprite.setPosition(cx + 10.f, currentY + 8.f);
        mEditSprites.push_back(editSprite);

        sf::Sprite delSprite(deleteTexture);
        delSprite.setPosition(cx + 80.f, currentY + 8.f);
        mDeleteSprites.push_back(delSprite);

        currentY += ROW_HEIGHT;
    }

    mTotalContentHeight = displayRows.size() * ROW_HEIGHT;
}

bool ManagementState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event, *getContext().window);
    sf::RenderWindow &window = *getContext().window;

    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        {
            float delta = event.mouseWheelScroll.delta;
            mScrollY -= delta * 30.f;
            updateScrollView();
        }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i mousePosRaw = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePosRaw, mTableView);

            for (size_t i = 0; i < mEditSprites.size(); ++i)
            {
                if (mEditSprites[i].getGlobalBounds().contains(mousePos))
                {
                    handleEdit(i);
                    return true;
                }
            }

            for (size_t i = 0; i < mDeleteSprites.size(); ++i)
            {
                if (mDeleteSprites[i].getGlobalBounds().contains(mousePos))
                {
                    handleDelete(i);
                    return true;
                }
            }
        }
    }

    return false;
}

void ManagementState::updateScrollView()
{
    float visibleHeight = mTableBounds.height;
    if (mScrollY < 0.f) mScrollY = 0.f;

    float maxScroll = std::max(0.f, mTotalContentHeight - visibleHeight);
    if (mScrollY > maxScroll) mScrollY = maxScroll;

    float listTopY = TABLE_Y + HEADER_HEIGHT;
    float defaultCenterY = listTopY + visibleHeight / 2.f;
    mTableView.setCenter(mTableBounds.width / 2.f, defaultCenterY + mScrollY);
}

bool ManagementState::update(sf::Time deltaTime)
{
    reloadTable(); // Keeping this to ensure UI refreshes
    return true;
}

void ManagementState::draw()
{
    sf::RenderWindow &window = *getContext().window;

    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);
    window.draw(mTableHeader);
    for (auto &t : mHeaderTexts) window.draw(t);

    window.setView(mTableView);
    for (auto &r : mRowRects) window.draw(r);
    for (auto &t : mRowTexts) window.draw(t);

    for (auto &s : mEditSprites) window.draw(s);
    for (auto &s : mDeleteSprites) window.draw(s);

    window.setView(window.getDefaultView());
    window.draw(mGUIContainer);
}

void ManagementState::handleEdit(int index)
{
    ManagementEditState::setPayload(index, static_cast<int>(currentMode),
                                    (currentMode == Mode::Income)
                                        ? &incomeTypeManager
                                    : (currentMode == Mode::Expense)
                                        ? &expenseTypeManager
                                        : &walletTypeManager);
    requestStackPush(States::ID::ManagementEdit);
}

void ManagementState::handleDelete(int index)
{
    if (currentMode == Mode::Income)
    {
        // Uses the remove() method. Note: Ensure IncomeManager.cpp implements logic!
        getContext().incomeManager->remove(index);
    }
    else
    {
        TypeManager *manager = (currentMode == Mode::Expense) ? &expenseTypeManager : &walletTypeManager;
        manager->removeTypeByIndex(index);
    }

    reloadTable();
}