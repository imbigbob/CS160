#include "TransactionState.hpp"

#include <../src/core/DynamicArray/DynamicArray.hpp>
#include <fstream>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"

// Define constants for easier adjustments
const float TABLE_X = 150.f;
const float TABLE_Y = 150.f;  // Header Y position
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

    // --- BUTTONS SETUP (Same as your code) ---
    auto backButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Back"
    );
    backButton->setPosition(250.f, 50.f);
    backButton->setCallback([this]() { requestStackPop(); });

    auto addButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Add"
    );
    addButton->setPosition(400.f, 50.f);
    addButton->setCallback([this]() { /* TODO: Add transaction */ });

    auto toggleButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Show Expense"
    );
    toggleButton->setPosition(550.f, 50.f);
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
    float colWidth[] = {180.f, 180.f, 120.f, 200.f};

    mTableHeader.setSize(
        sf::Vector2f(
            colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3], HEADER_HEIGHT
        )
    );
    mTableHeader.setPosition(TABLE_X, TABLE_Y);
    mTableHeader.setFillColor(sf::Color(230, 230, 230));

    sf::Font& font = context.fontHolder->get(Fonts::ID::Dosis);
    DynamicArray<std::string> headers;
    headers.pushBack("Date");
    headers.pushBack("Source");
    headers.pushBack("Amount");
    headers.pushBack("Description");

    float currentX = TABLE_X;

    for (int i = 0; i < headers.getSize(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(headers[i]);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setPosition(currentX + 10.f, TABLE_Y + 8.f);
        mHeaderTexts.push_back(text);
        currentX += colWidth[i];
    }

    // --- VIEW SETUP (SCROLLING MAGIC) ---
    // The list starts AFTER the header
    float listTopY = TABLE_Y + HEADER_HEIGHT;

    // The Visible Area (FloatRect)
    // Width: Full window width (or table width)
    // Height: From bottom of header to bottom of window
    mTableBounds = sf::FloatRect(
        0.f, listTopY, windowSize.x, windowSize.y - listTopY - 50.f
    );

    // Initialize the View to look at this area
    mTableView.setSize(mTableBounds.width, mTableBounds.height);
    mTableView.setCenter(
        mTableBounds.width / 2.f, listTopY + mTableBounds.height / 2.f
    );

    // Set Viewport: This maps the view to the specific part of the window
    // Normalized coordinates (0.0 to 1.0)
    float viewportY = listTopY / windowSize.y;
    float viewportHeight = (windowSize.y - listTopY - 50.f) / windowSize.y;
    mTableView.setViewport(sf::FloatRect(0.f, viewportY, 1.f, viewportHeight));

    // Initial Load
    reloadTable();
}

void TransactionState::reloadTable() {
    mRowRects.clear();
    mRowTexts.clear();

    // Reset Scroll on reload
    mScrollY = 0.f;
    updateScrollView();

    float tableX = TABLE_X;
    float colWidth[] = {180.f, 180.f, 120.f, 200.f};

    // Important: We start drawing at Y position relative to the View's start
    // The View starts at TABLE_Y + HEADER_HEIGHT.
    float startY = TABLE_Y + HEADER_HEIGHT;
    float currentY = startY;

    sf::Font& font = getContext().fontHolder->get(Fonts::ID::Dosis);

    // --- SELECT DATA ---
    // Note: Assuming DynamicArray has standard iterator or access
    int count = (mMode == Mode::Income) ? mIncomeManager.getAll().getSize()
                                        : mExpenseManager.getAll().getSize();

    for (int i = 0; i < count; i++) {
        // Retrieve data based on mode
        std::string date, name, amountStr, desc;

        if (mMode == Mode::Income) {
            const auto& r = mIncomeManager.getAll()[i];
            date = r.getDate();
            name = r.getName();
            amountStr = std::to_string(r.getAmount());
            desc = r.getDescription();
        } else {
            const auto& r = mExpenseManager.getAll()[i];
            date = r.getDate();
            name = r.getName();
            amountStr = std::to_string(r.getAmount());
            desc = r.getDescription();
        }

        // Row Background
        sf::RectangleShape rowRect;
        rowRect.setSize(
            sf::Vector2f(
                colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3],
                ROW_HEIGHT
            )
        );
        rowRect.setPosition(tableX, currentY);
        rowRect.setFillColor(sf::Color(245, 245, 245));
        mRowRects.push_back(rowRect);

        // Row Text
        DynamicArray<std::string> items;
        items.pushBack(date);
        items.pushBack(name);
        items.pushBack(amountStr);
        items.pushBack(desc);

        float cx = tableX;
        for (int col = 0; col < items.getSize(); col++) {
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

    // Calculate total height of the list logic
    mTotalContentHeight = count * ROW_HEIGHT;
}

bool TransactionState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);

    // Handle Scrolling
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            float delta = event.mouseWheelScroll.delta;
            float scrollSpeed = 30.f;
            mScrollY -= delta * scrollSpeed;

            updateScrollView();
        }
    }

    return false;
}

void TransactionState::updateScrollView() {
    // 1. Calculate the visible height of the table view
    float visibleHeight = mTableBounds.height;

    // 2. Clamp mScrollY (Don't scroll past top or bottom)
    if (mScrollY < 0.f) mScrollY = 0.f;

    // Max scroll is total content minus visible area
    // If content is smaller than screen, max scroll is 0
    float maxScroll = std::max(0.f, mTotalContentHeight - visibleHeight);

    if (mScrollY > maxScroll) mScrollY = maxScroll;

    // 3. Move the View
    // The view center starts at center of the rect + scroll offset
    // Original center Y = ListTopY + VisibleHeight / 2
    float listTopY = TABLE_Y + HEADER_HEIGHT;
    float defaultCenterY = listTopY + visibleHeight / 2.f;

    mTableView.setCenter(mTableBounds.width / 2.f, defaultCenterY + mScrollY);
}

bool TransactionState::update(sf::Time deltaTime) { return true; }

void TransactionState::draw() {
    sf::RenderWindow& window = *getContext().window;

    // 1. Draw Background
    window.setView(window.getDefaultView());  // Ensure we are in default view
    window.draw(mBackgroundSprite);

    // 2. Draw Header (Fixed position)
    window.draw(mTableHeader);
    for (auto& t : mHeaderTexts) window.draw(t);

    // 3. Draw Table Content (Scrolled View)
    window.setView(mTableView);  // <--- SWITCH CAMERA

    for (auto& r : mRowRects) window.draw(r);
    for (auto& t : mRowTexts) window.draw(t);

    // 4. Draw GUI Overlay (Buttons) - Switch back to default
    window.setView(window.getDefaultView());  // <--- RESET CAMERA
    window.draw(mGUIContainer);
}