#include "Program.hpp"

#include "../Global/Global.hpp"
#include "../State/MenuState/MenuState.hpp"
#include "../State/RecurringTransactionState/RecurringTransactionState.hpp"
#include "../State/StatisticsState/StatisticsState.hpp"
#include "../State/TransactionState/TransactionState.hpp"
Program::Program()
    : mWindow(
          sf::VideoMode(Global::WINDOW_WIDTH, Global::WINDOW_HEIGHT),
          "Financial Wallet", sf::Style::Close
      ),

      mStateStack(State::Context(mWindow, mTextureHolder, mFontHolder)) {
    mWindow.setKeyRepeatEnabled(false);

    loadTextures();
    loadFonts();

    registerStates();
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
    mTextureHolder.load(
        Textures::ID::TitleBackground, "assets/Textures/TitleBackground.png"
    );
    mTextureHolder.load(
        Textures::ID::MenuBackground, "assets/Textures/MenuBackground.png"
    );
    mTextureHolder.load(Textures::ID::Frame, "assets/Textures/Add.png");

    mTextureHolder.load(
        Textures::ID::ButtonBack, "assets/Textures/ButtonBack.png"
    );
    mTextureHolder.load(
        Textures::ID::ButtonNormal, "assets/Textures/ButtonNormal.png"
    );
    mTextureHolder.load(
        Textures::ID::ButtonSelected, "assets/Textures/ButtonSelected.png"
    );
    mTextureHolder.load(
        Textures::ID::ButtonPressed, "assets/Textures/ButtonPressed.png"
    );
    mTextureHolder.load(
        Textures::ID::LeftButtonNormal, "assets/Textures/LeftButtonNormal.png"
    );
    mTextureHolder.load(
        Textures::ID::RightButtonNormal, "assets/Textures/RightButtonNormal.png"
    );
    mTextureHolder.load(
        Textures::ID::RightButtonSelected,
        "assets/Textures/RightButtonSelected.png"
    );
    mTextureHolder.load(
        Textures::ID::LeftButtonSelected,
        "assets/Textures/LeftButtonSelected.png"
    );
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
    mStateStack.registerState<RecurringTransactionState>(
        States::ID::RecurringTransaction
    );
    mStateStack.registerState<StatisticsState>(States::ID::Statistics);
}

void Program::handleEvent(sf::Event& event) {
    while (mWindow.pollEvent(event)) {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
    }
}

void Program::update() { mStateStack.update(TIME_PER_FRAME); }

void Program::draw() {
    mWindow.clear(sf::Color::Black);
    mStateStack.draw();
    mWindow.display();
}
