#include "RecurringState.hpp"
#include <algorithm> // for std::max
#include <iostream>
#include "core/DynamicArray/DynamicArray.hpp"
#include "../RecurringEditState/RecurringEditState.hpp"
#include "../RecurringAddState/RecurringAddState.hpp"
// Layout Constants
const float TABLE_X = 100.f;
const float TABLE_Y = 150.f; // Header Y position
const float HEADER_HEIGHT = 40.f;
const float ROW_HEIGHT = 60.f;

RecurringState::RecurringState(StateStack &stack, Context context)
    : State(stack, context),
      mScrollY(0.f),
      currentMode(Mode::Income),
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
    modeSwitchButton->setPosition(500.f, 50.f);
    modeSwitchButton->setCallback([this]()
                                  {
        // Rotate Mode
        if (currentMode == Mode::Income)
            currentMode = Mode::Expense;
        else
            currentMode = Mode::Income;

        // Update Text
        if (currentMode == Mode::Income)
            modeSwitchButton->setText("Income");
        else if (currentMode == Mode::Expense)
            modeSwitchButton->setText("Expense");

        reloadTable(); });

    addButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Add");
    addButton->setPosition(750.f, 50.f);

    addButton->setCallback([this]()
                           { handleAdd(); });

    mGUIContainer.addComponent(backButton);
    mGUIContainer.addComponent(modeSwitchButton);
    mGUIContainer.addComponent(addButton);

    // --- TABLE HEADER SETUP ---
    // Column Widths: [0]=No., [1]=Name, [2]=ID (Optional)
    float colWidth[] = {80.f, 150.f, 100.f, 120.f, 220.f, 80.f, 160.f, 160.f, 160.f};

    float tableWidth = 0.f;
    for (float w : colWidth)
        tableWidth += w;

    mTableHeader.setSize({tableWidth, HEADER_HEIGHT});
    mTableHeader.setPosition(TABLE_X, TABLE_Y);
    mTableHeader.setFillColor(sf::Color(230, 230, 230));

    DynamicArray<std::string> headers;
    headers.pushBack("No.");
    headers.pushBack("Amount");
    headers.pushBack("Wallet ID");
    headers.pushBack("Type ID");
    headers.pushBack("Description");
    headers.pushBack("Day");
    headers.pushBack("Start Date");
    headers.pushBack("End Date");
    headers.pushBack("Actions");

    float currentX = TABLE_X;
    for (size_t i = 0; i < headers.getSize(); ++i)
    {
        sf::Text text;
        text.setFont(context.fontHolder->get(Fonts::ID::Dosis));
        text.setString(headers[i]);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
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
        mTableBounds.width / 2.f, listTopY + mTableBounds.height / 2.f);

    // Map logic to screen pixels (Viewport)
    float viewportY = listTopY / windowSize.y;
    float viewportHeight = visibleHeight / windowSize.y;
    mTableView.setViewport(sf::FloatRect(0.f, viewportY, 1.f, viewportHeight));

    // Initial Load
    reloadTable();
}

void RecurringState::handleAdd()
{
    RecurringAddState::setPayload(static_cast<int>(currentMode),
                                  (currentMode == Mode::Income)
                                      ? &recurringManager.getIncomes()
                                      : &recurringManager.getExpenses());
    requestStackPush(States::ID::RecurringAdd);
    if (currentMode == Mode::Income)
        recurringManager.updateIncomeDB();
    else if (currentMode == Mode::Expense)
        recurringManager.updateExpenseDB();
}
void RecurringState::reloadTable()
{

    mRowRects.clear();
    mRowTexts.clear();
    // Clear old icons
    mEditSprites.clear();
    mDeleteSprites.clear();

    mScrollY = 0.f;
    updateScrollView();

    // Select manager (Same as your code)
    DynamicArray<RecurringTransaction> const *dataList = nullptr;
    if (currentMode == Mode::Income)
        dataList = &recurringManager.getIncomes();
    else if (currentMode == Mode::Expense)
        dataList = &recurringManager.getExpenses();

    auto &types = *dataList;

    float tableX = TABLE_X;
    float colWidth[] = {80.f, 150.f, 100.f, 120.f, 220.f, 80.f, 160.f, 160.f, 160.f}; // Added 3rd col width logic
    float currentY = TABLE_Y + HEADER_HEIGHT;

    sf::Font &font = getContext().fontHolder->get(Fonts::ID::Dosis);

    // GET TEXTURES (Ensure you have these in your TextureHolder)
    // If you don't have IDs yet, assume Textures::ID::EditIcon and Textures::ID::DeleteIcon
    const sf::Texture &editTexture = getContext().textureHolder->get(Textures::ID::Edit);
    const sf::Texture &deleteTexture = getContext().textureHolder->get(Textures::ID::Bin);

    for (int i = 0; i < types.getSize(); i++)
    {
        // 1. Background Row (Same as your code)
        sf::RectangleShape rowRect;
        rowRect.setSize(sf::Vector2f(colWidth[0] + colWidth[1] + colWidth[2] + colWidth[3] + colWidth[4] + colWidth[5] + colWidth[6] + colWidth[7] + colWidth[8], ROW_HEIGHT));
        rowRect.setPosition(tableX, currentY);
        rowRect.setFillColor((i % 2 == 0) ? sf::Color(250, 250, 250) : sf::Color(240, 240, 240));
        mRowRects.pushBack(rowRect);

        // 2. Text Data (Same as your code)
        std::string orderStr = std::to_string(i + 1);
        std::string amountStr = std::to_string(types[i].amount);
        std::string walletIdStr = types[i].walletId;
        std::string typeIdStr = types[i].categoryId;
        std::string descriptionStr = types[i].description;
        std::string day = std::to_string(types[i].day);
        std::string startDateStr = types[i].startDate;
        std::string endDateStr = types[i].endDate;

        std::vector<std::string>
            rowData = {orderStr, amountStr, walletIdStr, typeIdStr, descriptionStr, day, startDateStr, endDateStr};

        float cx = tableX;
        for (int col = 0; col < rowData.size(); col++)
        {
            sf::Text t;
            t.setFont(font);
            t.setString(rowData[col]);
            t.setCharacterSize(25);
            t.setFillColor(sf::Color::Black);
            t.setPosition(cx + 10.f, currentY + 8.f);
            mRowTexts.pushBack(t);
            cx += colWidth[col];
        }

        // --- 3. ADD ICONS (Column 3) ---
        // cx is now at the start of the 3rd column (approx X = 580)

        // Setup Edit Icon
        sf::Sprite editSprite(editTexture);
        // Scale if texture is too big (optional, e.g., fit to 24x24)
        // editSprite.setScale(24.f / editTexture.getSize().x, 24.f / editTexture.getSize().y);
        editSprite.setPosition(cx + 10.f, currentY + 8.f);
        mEditSprites.pushBack(editSprite);

        // Setup Delete Icon (Position it after Edit icon)
        sf::Sprite delSprite(deleteTexture);
        // delSprite.setScale(24.f / deleteTexture.getSize().x, 24.f / deleteTexture.getSize().y);
        delSprite.setPosition(cx + 80.f, currentY + 8.f); // Offset x by 40-50px
        mDeleteSprites.pushBack(delSprite);

        currentY += ROW_HEIGHT;
    }

    mTotalContentHeight = types.getSize() * ROW_HEIGHT;
}

bool RecurringState::handleEvent(const sf::Event &event)
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

    // --- HANDLE ICON CLICKS ---
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // 1. Get Mouse Position relative to the window
            sf::Vector2i mousePosRaw = sf::Mouse::getPosition(window);

            // 2. Convert to World Coordinates inside the Table View
            // This accounts for the scroll position automatically!
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePosRaw, mTableView);

            // 3. Check collisions with Edit Buttons
            for (size_t i = 0; i < mEditSprites.getSize(); ++i)
            {
                if (mEditSprites[i].getGlobalBounds().contains(mousePos))
                {
                    handleEdit(i);
                    return true; // Input handled
                }
            }

            // 4. Check collisions with Delete Buttons
            for (size_t i = 0; i < mDeleteSprites.getSize(); ++i)
            {
                if (mDeleteSprites[i].getGlobalBounds().contains(mousePos))
                {
                    handleDelete(i);
                    return true; // Input handled
                }
            }
        }
    }

    return false;
}

void RecurringState::updateScrollView()
{
    float visibleHeight = mTableBounds.height;
    if (mScrollY < 0.f)
        mScrollY = 0.f;

    float maxScroll = std::max(0.f, mTotalContentHeight - visibleHeight);
    if (mScrollY > maxScroll)
        mScrollY = maxScroll;

    // Move the view center
    float listTopY = TABLE_Y + HEADER_HEIGHT;
    float defaultCenterY = listTopY + visibleHeight / 2.f;
    mTableView.setCenter(mTableBounds.width / 2.f, defaultCenterY + mScrollY);
}

bool RecurringState::update(sf::Time deltaTime)
{
    reloadTable();
    return true;
}

void RecurringState::draw()
{
    sf::RenderWindow &window = *getContext().window;

    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);
    window.draw(mTableHeader);
    for (size_t i = 0; i < mHeaderTexts.getSize(); ++i)
        window.draw(mHeaderTexts[i]);

    window.setView(mTableView);

    for (size_t i = 0; i < mRowRects.getSize(); ++i)
        window.draw(mRowRects[i]);
    for (size_t i = 0; i < mRowTexts.getSize(); ++i)
        window.draw(mRowTexts[i]);

    for (size_t i = 0; i < mEditSprites.getSize(); ++i)
        window.draw(mEditSprites[i]);
    for (size_t i = 0; i < mDeleteSprites.getSize(); ++i)
        window.draw(mDeleteSprites[i]);

    // 3. Reset to Standard View
    window.setView(window.getDefaultView());
    window.draw(mGUIContainer);
}

void RecurringState::handleEdit(int index)
{
    RecurringEditState::setPayload(index, (currentMode == Mode::Income) ? &recurringManager.getIncomes() : &recurringManager.getExpenses());
    requestStackPush(States::ID::RecurringEdit);
    if (currentMode == Mode::Income)
        recurringManager.updateIncomeDB();
    else if (currentMode == Mode::Expense)
        recurringManager.updateExpenseDB();
}

void RecurringState::handleDelete(int index)
{
    DynamicArray<RecurringTransaction> *dataList = nullptr;
    if (currentMode == Mode::Income)
        dataList = &recurringManager.getIncomes();
    else if (currentMode == Mode::Expense)
        dataList = &recurringManager.getExpenses();

    dataList->removeAt(index);

    if (currentMode == Mode::Income)
        recurringManager.updateIncomeDB();
    else if (currentMode == Mode::Expense)
        recurringManager.updateExpenseDB();

    reloadTable();
}