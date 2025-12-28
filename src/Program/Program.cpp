#include "Program.hpp"

#include "../Global/Global.hpp"

// --- Include General States ---
#include "../State/MenuState/MenuState.hpp"
#include "../State/TransactionState/TransactionState.hpp"
#include "../State/StatisticsState/StatisticsState.hpp"

// --- Include NEW Master Management States ---
#include "../State/MasterManagementState/MasterManagementState.hpp"
#include "../State/MasterManagementAddState/MasterManagementAddState.hpp"
#include "../State/MasterManagementEditState/MasterManagementEditState.hpp"

// --- Include NEW Recurring States ---
#include "../State/RecurringState/RecurringState.hpp"
#include "../State/RecurringAddState/RecurringAddState.hpp"
#include "../State/RecurringEditState/RecurringEditState.hpp"

// --- Include Recurring Manager Logic ---
#include "../core/RecurringManager/RecurringManager.hpp"

const sf::Time Program::TIME_PER_FRAME = sf::seconds(1.f / 60.f);

Program::Program()
    : mWindow(sf::VideoMode(Global::WINDOW_WIDTH, Global::WINDOW_HEIGHT),
              "Financial Wallet", sf::Style::Close),
      
      // 1. Initialize OLD Managers
      mIncomeManager(),
      mExpenseManager(),

      // 2. Initialize NEW Managers (Loads JSONs automatically)
      mIncomeManagement(),
      mExpenseManagement(),
      mWalletManager(),

      // 3. Initialize Shared Variables
      mActiveRecordType(0),
      mActiveRecordId(-1),

      // 4. Initialize StateStack with FULL Context
      mStateStack(State::Context(mWindow, mTextureHolder, mFontHolder,
                                 mIncomeManager, mExpenseManager,          
                                 mIncomeManagement, mExpenseManagement, mWalletManager,
                                 mActiveRecordType, mActiveRecordId))      
{
    mWindow.setKeyRepeatEnabled(false);

    loadTextures();
    loadFonts();

    // --- AUTOMATIC CHECK ---
    // Check for recurring transactions due today upon startup
    RecurringManager recurringMgr;
    recurringMgr.processDailyCheck(mIncomeManager, mExpenseManager);

    registerStates();
    
    // Start at Menu
    mStateStack.pushState(States::ID::Menu);
}

Program::~Program() {}

void Program::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate;
    sf::Event event;

    while (mWindow.isOpen()) {
        handleEvent(event);
        timeSinceLastUpdate += clock.restart();

        while (timeSinceLastUpdate > TIME_PER_FRAME) {
            timeSinceLastUpdate -= TIME_PER_FRAME;
            handleEvent(event);
            update();

            if (mStateStack.isEmpty()) {
                mWindow.close();
            }
        }

        draw();
    }
}

void Program::loadTextures() {
    // Backgrounds
    mTextureHolder.load(Textures::ID::MenuBackground, "assets/Textures/MenuBackground.jpg");
    mTextureHolder.load(Textures::ID::StatBackground, "assets/Textures/StatBackground.jpg");
    mTextureHolder.load(Textures::ID::ManagementBackground, "assets/Textures/ManagementBackground.jpg");

    // Icons
    mTextureHolder.load(Textures::ID::Bin, "assets/Textures/Bin.png");
    mTextureHolder.load(Textures::ID::Edit, "assets/Textures/Edit.png");

    // UI Buttons
    mTextureHolder.load(Textures::ID::ButtonBack, "assets/Textures/ButtonBack.png");
    mTextureHolder.load(Textures::ID::ButtonNormal, "assets/Textures/ButtonNormal.png");
    mTextureHolder.load(Textures::ID::ButtonSelected, "assets/Textures/ButtonSelected.png");
    mTextureHolder.load(Textures::ID::ButtonPressed, "assets/Textures/ButtonPressed.png");
    
    // Arrows/Navigation
    mTextureHolder.load(Textures::ID::LeftButtonNormal, "assets/Textures/LeftButtonNormal.png");
    mTextureHolder.load(Textures::ID::RightButtonNormal, "assets/Textures/RightButtonNormal.png");
    mTextureHolder.load(Textures::ID::RightButtonSelected, "assets/Textures/RightButtonSelected.png");
    mTextureHolder.load(Textures::ID::LeftButtonSelected, "assets/Textures/LeftButtonSelected.png");
}

void Program::loadFonts() {
    mFontHolder.load(Fonts::ID::Dosis, "assets/Fonts/Dosis.ttf");
    mFontHolder.load(Fonts::ID::Pacifico, "assets/Fonts/Pacifico-Regular.ttf");
    mFontHolder.load(Fonts::ID::VTV323, "assets/Fonts/VT323-Regular.ttf");
    mFontHolder.load(Fonts::ID::Minecraft, "assets/Fonts/Minecraft.ttf");
}

void Program::registerStates() {
    mStateStack.registerState<MenuState>(States::ID::Menu);
    mStateStack.registerState<TransactionState>(States::ID::Transaction);
    mStateStack.registerState<StatisticsState>(States::ID::Statistics);

    // --- Management States ---
    mStateStack.registerState<MasterManagementState>(States::ID::Management);
    mStateStack.registerState<MasterManagementAddState>(States::ID::ManagementAdd);
    mStateStack.registerState<MasterManagementEditState>(States::ID::ManagementEdit);

    // --- Recurring States ---
    // Note: States::ID::RecurringTransaction maps to the "Recurring" button in Menu
    mStateStack.registerState<RecurringState>(States::ID::RecurringTransaction);
    mStateStack.registerState<RecurringAddState>(States::ID::RecurringAdd);
    mStateStack.registerState<RecurringEditState>(States::ID::RecurringEdit);
}

void Program::handleEvent(sf::Event& event) {
    while (mWindow.pollEvent(event)) {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
    }
}

void Program::update() { 
    mStateStack.update(TIME_PER_FRAME); 
}

void Program::draw() {
    mWindow.clear(sf::Color::Black);
    mStateStack.draw();
    mWindow.display();
}