#include "ManagementState.hpp"

#include <../src/core/DynamicArray/DynamicArray.hpp>
#include <algorithm>  // for std::max
// Layout Constants
const float TABLE_X = 200.f;
const float TABLE_Y = 150.f;  // Header Y position
const float HEADER_HEIGHT = 40.f;
const float ROW_HEIGHT = 40.f;

ManagementState::ManagementState(StateStack& stack, Context context)
    : State(stack, context),
      currentMode(Mode::Income),
      mScrollY(0.f),
      mTotalContentHeight(0.f) {
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

    modeSwitchButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Income"
    );
    modeSwitchButton->setPosition(450.f, 50.f);
    modeSwitchButton->setCallback([this]() {
        // Rotate Mode
        if (currentMode == Mode::Income)
            currentMode = Mode::Expense;
        else if (currentMode == Mode::Expense)
            currentMode = Mode::Wallet;
        else
            currentMode = Mode::Income;

        // Update Text
        if (currentMode == Mode::Income)
            modeSwitchButton->setText("Income");
        else if (currentMode == Mode::Expense)
            modeSwitchButton->setText("Expense");
        else
            modeSwitchButton->setText("Wallet");

        reloadTable();
    });

    addButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Add"
    );
    addButton->setPosition(700.f, 50.f);
    addButton->setCallback([this]() {
        // requestStackPush(States::ID::AddType);
    });

    mGUIContainer.addComponent(backButton);
    mGUIContainer.addComponent(modeSwitchButton);
    mGUIContainer.addComponent(addButton);

    // --- TABLE HEADER SETUP ---
    // Column Widths: [0]=No., [1]=Name, [2]=ID (Optional)
    float colWidth[] = {80.f, 300.f};

    mTableHeader.setSize(
        sf::Vector2f(colWidth[0] + colWidth[1], HEADER_HEIGHT)
    );
    mTableHeader.setPosition(TABLE_X, TABLE_Y);
    mTableHeader.setFillColor(sf::Color(230, 230, 230));

    sf::Font& font = context.fontHolder->get(Fonts::ID::Dosis);
    DynamicArray<std::string> headers;
    headers.pushBack("No.");
    headers.pushBack("Name");

    float currentX = TABLE_X;
    for (int i = 0; i < headers.getSize(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(headers[i]);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        // Center text vertically in header
        text.setPosition(currentX + 10.f, TABLE_Y + 8.f);
        mHeaderTexts.pushBack(text);

        currentX += colWidth[i];
    }

    // --- VIEW SETUP (SCROLLING) ---
    float listTopY = TABLE_Y + HEADER_HEIGHT;
    float bottomMargin = 50.f;
    float visibleHeight = windowSize.y - listTopY - bottomMargin;

    // Define the logical area we want to look at
    mTableBounds = sf::FloatRect(0.f, listTopY, windowSize.x, visibleHeight);

    mTableView.setSize(mTableBounds.width, mTableBounds.height);
    mTableView.setCenter(
        mTableBounds.width / 2.f, listTopY + mTableBounds.height / 2.f
    );

    // Map logic to screen pixels (Viewport)
    float viewportY = listTopY / windowSize.y;
    float viewportHeight = visibleHeight / windowSize.y;
    mTableView.setViewport(sf::FloatRect(0.f, viewportY, 1.f, viewportHeight));

    // Initial Load
    reloadTable();
}

void ManagementState::reloadTable() {
    mRowRects.clear();
    mRowTexts.clear();
    mScrollY = 0.f;
    updateScrollView();

    // Select the correct manager
    TypeManager* manager = nullptr;
    if (currentMode == Mode::Income)
        manager = &incomeTypeManager;
    else if (currentMode == Mode::Expense)
        manager = &expenseTypeManager;
    else
        manager = &walletTypeManager;

    auto& types = manager->getAllTypes();

    float tableX = TABLE_X;
    float colWidth[] = {80.f, 300.f};

    // Start drawing rows relative to where the View starts (below header)
    float currentY = TABLE_Y + HEADER_HEIGHT;

    sf::Font& font = getContext().fontHolder->get(Fonts::ID::Dosis);

    for (int i = 0; i < types.getSize(); i++) {
        // --- 1. Background Row ---
        sf::RectangleShape rowRect;
        rowRect.setSize(sf::Vector2f(colWidth[0] + colWidth[1], ROW_HEIGHT));
        rowRect.setPosition(tableX, currentY);

        // Alternating colors for readability
        if (i % 2 == 0)
            rowRect.setFillColor(sf::Color(250, 250, 250));
        else
            rowRect.setFillColor(sf::Color(240, 240, 240));

        mRowRects.push_back(rowRect);

        // --- 2. Prepare Data ---
        // Column 0: Order (1, 2, 3...)
        std::string orderStr = std::to_string(i + 1);
        // Column 1: Name
        std::string nameStr = types[i].getName();
        // Column 2: ID (Optional, assuming types have getID, or blank)
        std::string idStr = "";  // types[i].getID();

        DynamicArray<std::string> rowData;
        rowData.pushBack(orderStr);
        rowData.pushBack(nameStr);
        rowData.pushBack(idStr);

        // --- 3. Create Text ---
        float cx = tableX;
        for (int col = 0; col < rowData.getSize(); col++) {
            sf::Text t;
            t.setFont(font);
            t.setString(rowData[col]);
            t.setCharacterSize(18);
            t.setFillColor(sf::Color::Black);
            t.setPosition(cx + 10.f, currentY + 8.f);

            mRowTexts.push_back(t);
            cx += colWidth[col];
        }

        currentY += ROW_HEIGHT;
    }

    mTotalContentHeight = types.getSize() * ROW_HEIGHT;
}

bool ManagementState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);

    // Handle Scrolling
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            float delta = event.mouseWheelScroll.delta;
            mScrollY -= delta * 30.f;  // Scroll speed
            updateScrollView();
        }
    }
    return false;
}

void ManagementState::updateScrollView() {
    float visibleHeight = mTableBounds.height;
    if (mScrollY < 0.f) mScrollY = 0.f;

    float maxScroll = std::max(0.f, mTotalContentHeight - visibleHeight);
    if (mScrollY > maxScroll) mScrollY = maxScroll;

    // Move the view center
    float listTopY = TABLE_Y + HEADER_HEIGHT;
    float defaultCenterY = listTopY + visibleHeight / 2.f;
    mTableView.setCenter(mTableBounds.width / 2.f, defaultCenterY + mScrollY);
}

bool ManagementState::update(sf::Time deltaTime) { return true; }

void ManagementState::draw() {
    sf::RenderWindow& window = *getContext().window;

    // 1. Standard View (Background + Headers + Buttons)
    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);

    // Draw Header
    window.draw(mTableHeader);
    for (int i = 0; i < mHeaderTexts.getSize(); i++) {
        window.draw(mHeaderTexts[i]);
    }
    // 2. Table View (Rows)
    window.setView(mTableView);
    for (auto& r : mRowRects) window.draw(r);
    for (auto& t : mRowTexts) window.draw(t);

    // 3. Reset to Standard View for Buttons
    window.setView(window.getDefaultView());
    window.draw(mGUIContainer);
}