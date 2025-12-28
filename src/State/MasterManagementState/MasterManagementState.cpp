#include "MasterManagementState.hpp"
#include "../../ResourceHolder/ResourceHolder.hpp"

// Managers
#include "../../core/IncomeManagement/IncomeManagement.hpp"
#include "../../core/ExpenseManagement/ExpenseManagement.hpp"
#include "../../core/WalletManager/WalletManager.hpp"

// States
#include "../MasterManagementAddState/MasterManagementAddState.hpp" 
#include "../MasterManagementEditState/MasterManagementEditState.hpp" 

MasterManagementState::MasterManagementState(StateStack& stack, Context context)
    : State(stack, context)
    , mCurrentMode(Mode::Expense)
    , mListContainer(std::make_shared<GUI::Container>())
    , mPendingDeleteId(-1) // <--- Initialize to -1 (no pending delete)
{
    sf::Vector2f windowSize(context.window->getSize());

    // 1. Background & Title
    mBackgroundSprite.setTexture(context.textureHolder->get(Textures::ID::MenuBackground));
    mBackgroundSprite.setScale(
        windowSize.x / mBackgroundSprite.getLocalBounds().width,
        windowSize.y / mBackgroundSprite.getLocalBounds().height
    );

    mTitleText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mTitleText.setString("Expense Categories");
    mTitleText.setCharacterSize(40);
    mTitleText.setFillColor(sf::Color::White);
    mTitleText.setPosition(50.f, 30.f);

    // 2. Tab Buttons
    auto makeTabBtn = [&](const std::string& label, float x, Mode mode) {
        auto btn = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, label);
        btn->setPosition(x, 100.f);
        btn->setCallback([this, mode, label]() {
            mCurrentMode = mode;
            mTitleText.setString(label);
            updateDisplayedText();
        });
        mGUIContainer.addComponent(btn);
    };

    makeTabBtn("Expenses", 100.f, Mode::Expense);
    makeTabBtn("Income", 300.f, Mode::Income);
    makeTabBtn("Wallets", 500.f, Mode::Wallet);

    // 3. Action Buttons
    auto backBtn = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Back");
    backBtn->setPosition(windowSize.x - 200.f, windowSize.y - 80.f);
    backBtn->setCallback([this]() { requestStackPop(); });

    auto addBtn = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Add New");
    addBtn->setPosition(windowSize.x - 400.f, windowSize.y - 80.f);
    addBtn->setCallback([this]() {
        *getContext().activeRecordType = (int)mCurrentMode;
        requestStackPush(States::ID::ManagementAdd);
    });

    mGUIContainer.addComponent(backBtn);
    mGUIContainer.addComponent(addBtn);

    updateDisplayedText();
}

void MasterManagementState::updateDisplayedText() {
    mListItems.clear();
    mListContainer = std::make_shared<GUI::Container>(); 

    const sf::Font& font = getContext().fontHolder->get(Fonts::ID::Dosis);
    
    // --- UI FIX: Adjusted Coordinates to prevent overlapping ---
    float startX = 50.f;      // Moved list slightly left
    float startY = 180.f;
    float boxWidth = 650.f;   // Made box wider
    float boxHeight = 50.f;
    float padding = 10.f; 

    auto addListItem = [&](int id, const std::string& name, int index) {
        ListItem item;
        float yPos = startY + index * (boxHeight + padding);

        // A. Background Box
        item.background.setSize(sf::Vector2f(boxWidth, boxHeight));
        item.background.setFillColor(sf::Color(255, 255, 255, 200)); 
        item.background.setPosition(startX, yPos);
        item.background.setOutlineThickness(1.f);
        item.background.setOutlineColor(sf::Color::Black);

        // B. Text
        item.text.setFont(font);
        item.text.setString(name); 
        item.text.setCharacterSize(24);
        item.text.setFillColor(sf::Color::Black); 
        
        sf::FloatRect bounds = item.text.getLocalBounds();
        item.text.setOrigin(bounds.left, bounds.top + bounds.height / 2.0f);
        item.text.setPosition(startX + 20.f, yPos + boxHeight / 2.0f);

        // C. Edit Button (Moved further right)
        item.btnEdit = std::make_shared<GUI::Button>(*getContext().fontHolder, *getContext().textureHolder, "Edit");
        item.btnEdit->setPosition(startX + boxWidth + 20.f, yPos); // 20px gap from box
        item.btnEdit->setCallback([this, id]() {
            *getContext().activeRecordType = (int)mCurrentMode;
            *getContext().activeRecordId = id; 
            requestStackPush(States::ID::ManagementEdit);
        });

        // D. Delete Button (Moved even further right)
        item.btnDelete = std::make_shared<GUI::Button>(*getContext().fontHolder, *getContext().textureHolder, "Del");
        item.btnDelete->setPosition(startX + boxWidth + 150.f, yPos); // 130px gap from Edit button
        item.btnDelete->setCallback([this, id]() {
            // --- CRASH FIX: Do NOT delete immediately. Schedule it. ---
            mPendingDeleteId = id;
        });

        mListContainer->addComponent(item.btnEdit);
        mListContainer->addComponent(item.btnDelete);
        mListItems.push_back(item);
    };

    // Load Data
    if (mCurrentMode == Mode::Expense) {
        for (const auto& i : getContext().expenseManagement->getAll()) addListItem(i.id, i.name, mListItems.size());
    }
    else if (mCurrentMode == Mode::Income) {
        for (const auto& i : getContext().incomeManagement->getAll()) addListItem(i.id, i.name, mListItems.size());
    }
    else if (mCurrentMode == Mode::Wallet) {
        for (const auto& i : getContext().walletManager->getAll()) addListItem(i.id, i.name, mListItems.size());
    }
}

void MasterManagementState::deleteItem(int id) {
    bool success = false;
    if (mCurrentMode == Mode::Expense) success = getContext().expenseManagement->deleteExpenseType(id);
    else if (mCurrentMode == Mode::Income) success = getContext().incomeManagement->deleteIncomeType(id);
    else if (mCurrentMode == Mode::Wallet) success = getContext().walletManager->deleteWallet(id);

    if (success) {
        updateDisplayedText(); 
    }
}

bool MasterManagementState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);
    mListContainer->handleEvent(event, *getContext().window);
    return false;
}

bool MasterManagementState::update(sf::Time deltaTime) {
    // --- CRASH FIX: Check for pending deletion safely here ---
    if (mPendingDeleteId != -1) {
        deleteItem(mPendingDeleteId);
        mPendingDeleteId = -1; // Reset after deleting
    }
    return true;
}

void MasterManagementState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    
    for (const auto& item : mListItems) {
        window.draw(item.background);
        window.draw(item.text);
    }
    
    window.draw(*mListContainer);
    window.draw(mTitleText);
    window.draw(mGUIContainer); 
}