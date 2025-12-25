#include "ManagementState.hpp"
#include "../ManagementEditState/ManagementEditState.hpp"
#include "../ManagementAddState/ManagementAddState.hpp"
#include <algorithm> // for std::max
#include <iostream>
// Layout Constants
const float TABLE_X = 200.f;
const float TABLE_Y = 150.f; // Header Y position
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
        context.textureHolder->get(Textures::ID::MenuBackground);

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

        reloadTable(); });

    addButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Add");
    addButton->setPosition(700.f, 50.f);

    addButton->setCallback([this]()
                           { handleAdd(); });

    mGUIContainer.addComponent(backButton);
    mGUIContainer.addComponent(modeSwitchButton);
    mGUIContainer.addComponent(addButton);

    // --- TABLE HEADER SETUP ---
    // Column Widths: [0]=No., [1]=Name, [2]=ID (Optional)
    float colWidth[] = {
        80.f,
        300.f,
        200.f};

    mTableHeader.setSize(
        sf::Vector2f(colWidth[0] + colWidth[1] + colWidth[2], HEADER_HEIGHT));
    mTableHeader.setPosition(TABLE_X, TABLE_Y);
    mTableHeader.setFillColor(sf::Color(230, 230, 230));

    sf::Font &font = context.fontHolder->get(Fonts::ID::Dosis);
    std::vector<std::string> headers = {"No.", "Name", ""};

    float currentX = TABLE_X;
    for (int i = 0; i < headers.size(); i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(headers[i]);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        // Center text vertically in header
        text.setPosition(currentX + 10.f, TABLE_Y + 8.f);
        mHeaderTexts.push_back(text);

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

void ManagementState::handleAdd()
{
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
    for (int i = 0; i < incomeTypeManager.getAllTypes().getSize(); i++)
    {
        std::cout << "Income Type " << i << ": " << incomeTypeManager.getAllTypes()[i].getName() << std::endl;
    }
    mRowRects.clear();
    mRowTexts.clear();
    // Clear old icons
    mEditSprites.clear();
    mDeleteSprites.clear();

    mScrollY = 0.f;
    updateScrollView();

    // Select manager (Same as your code)
    TypeManager *manager = nullptr;
    if (currentMode == Mode::Income)
        manager = &incomeTypeManager;
    else if (currentMode == Mode::Expense)
        manager = &expenseTypeManager;
    else
        manager = &walletTypeManager;

    auto &types = manager->getAllTypes();

    float tableX = TABLE_X;
    float colWidth[] = {80.f, 300.f, 200.f}; // Added 3rd col width logic
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
        rowRect.setSize(sf::Vector2f(colWidth[0] + colWidth[1] + colWidth[2], ROW_HEIGHT));
        rowRect.setPosition(tableX, currentY);
        rowRect.setFillColor((i % 2 == 0) ? sf::Color(250, 250, 250) : sf::Color(240, 240, 240));
        mRowRects.push_back(rowRect);

        // 2. Text Data (Same as your code)
        std::string orderStr = std::to_string(i + 1);
        std::string nameStr = types[i].getName();
        std::vector<std::string> rowData = {orderStr, nameStr};

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

        // --- 3. ADD ICONS (Column 3) ---
        // cx is now at the start of the 3rd column (approx X = 580)

        // Setup Edit Icon
        sf::Sprite editSprite(editTexture);
        // Scale if texture is too big (optional, e.g., fit to 24x24)
        // editSprite.setScale(24.f / editTexture.getSize().x, 24.f / editTexture.getSize().y);
        editSprite.setPosition(cx + 10.f, currentY + 8.f);
        mEditSprites.push_back(editSprite);

        // Setup Delete Icon (Position it after Edit icon)
        sf::Sprite delSprite(deleteTexture);
        // delSprite.setScale(24.f / deleteTexture.getSize().x, 24.f / deleteTexture.getSize().y);
        delSprite.setPosition(cx + 80.f, currentY + 8.f); // Offset x by 40-50px
        mDeleteSprites.push_back(delSprite);

        currentY += ROW_HEIGHT;
    }

    mTotalContentHeight = types.getSize() * ROW_HEIGHT;
}

bool ManagementState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event, *getContext().window);
    sf::RenderWindow &window = *getContext().window;

    // Handle Scrolling (Same as your code)
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
            for (size_t i = 0; i < mEditSprites.size(); ++i)
            {
                if (mEditSprites[i].getGlobalBounds().contains(mousePos))
                {
                    handleEdit(i);
                    return true; // Input handled
                }
            }

            // 4. Check collisions with Delete Buttons
            for (size_t i = 0; i < mDeleteSprites.size(); ++i)
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

void ManagementState::updateScrollView()
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

bool ManagementState::update(sf::Time deltaTime)
{
    reloadTable();
    return true;
}

void ManagementState::draw()
{
    sf::RenderWindow &window = *getContext().window;

    // 1. Standard View
    window.setView(window.getDefaultView());
    window.draw(mBackgroundSprite);
    window.draw(mTableHeader);
    for (auto &t : mHeaderTexts)
        window.draw(t);

    // 2. Table View (Rows)
    window.setView(mTableView);

    for (auto &r : mRowRects)
        window.draw(r);
    for (auto &t : mRowTexts)
        window.draw(t);

    // --- DRAW ICONS ---
    for (auto &s : mEditSprites)
        window.draw(s);
    for (auto &s : mDeleteSprites)
        window.draw(s);

    // 3. Reset to Standard View
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
    // Select the correct manager
    TypeManager *manager = nullptr;
    if (currentMode == Mode::Income)
        manager = &incomeTypeManager;
    else if (currentMode == Mode::Expense)
        manager = &expenseTypeManager;
    else
        manager = &walletTypeManager;

    // Remove from manager
    // Assuming your manager has a remove method taking an index or ID
    manager->removeTypeByIndex(index);

    // IMPORTANT: Refresh table immediately to update UI
    reloadTable();
}