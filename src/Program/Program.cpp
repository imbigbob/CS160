#include "Program.hpp"

#include "../Global/Global.hpp"
#include "../State/MenuState/MenuState.hpp"
#include "../State/SettingsState/SettingsState.hpp"
#include "../State/TitleState/TitleState.hpp"
Program::Program()
    : mWindow(
          sf::VideoMode(Global::WINDOW_WIDTH, Global::WINDOW_HEIGHT),
          "Crossy Road", sf::Style::Close
      ),

      mStateStack(State::Context(mWindow, mTextureHolder, mFontHolder)) {
    mWindow.setKeyRepeatEnabled(false);

    loadTextures();
    loadFonts();
    loadSettings();

    registerStates();
    mStateStack.pushState(States::ID::Title);
}

Program::~Program() { saveSettings(); }

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

void Program::saveSettings() const {
    std::ofstream fout("data/Settings.txt");

    fout.close();
}

void Program::loadTextures() {
    mTextureHolder.load(
        Textures::ID::PlayerChoice1, "assets/Textures/BlackNinja.png"
    );
    mTextureHolder.load(
        Textures::ID::PlayerChoice2, "assets/Textures/BlueNinja.png"
    );
    mTextureHolder.load(
        Textures::ID::PlayerChoice3, "assets/Textures/GrayNinja.png"
    );
    mTextureHolder.load(
        Textures::ID::PlayerChoice4, "assets/Textures/GreenNinja.png"
    );
    mTextureHolder.load(
        Textures::ID::PlayerChoice5, "assets/Textures/RedNinja.png"
    );
    mTextureHolder.load(
        Textures::ID::PlayerChoice6, "assets/Textures/YellowNinja.png"
    );
    mTextureHolder.load(Textures::ID::Halo, "assets/Textures/Halo.png");

    mTextureHolder.load(
        Textures::ID::TitleBackground, "assets/Textures/TitleBackground.png"
    );
    mTextureHolder.load(
        Textures::ID::MenuBackground, "assets/Textures/MenuBackground.png"
    );

    mTextureHolder.load(
        Textures::ID::SettingBackground, "assets/Textures/SettingBackground.png"
    );
    mTextureHolder.load(Textures::ID::Frame, "assets/Textures/Frame.png");

    mTextureHolder.load(Textures::ID::Bus, "assets/Textures/Bus.png");
    mTextureHolder.load(Textures::ID::Car, "assets/Textures/Car.png");
    mTextureHolder.load(
        Textures::ID::PoliceCar, "assets/Textures/PoliceCar.png"
    );
    mTextureHolder.load(Textures::ID::Train, "assets/Textures/Train.png");
    mTextureHolder.load(Textures::ID::Van, "assets/Textures/Van.png");

    mTextureHolder.load(Textures::ID::Cat, "assets/Textures/Cat.png");
    mTextureHolder.load(Textures::ID::Cow, "assets/Textures/Cow.png");
    mTextureHolder.load(Textures::ID::Dog, "assets/Textures/Dog.png");
    mTextureHolder.load(Textures::ID::Horse, "assets/Textures/Horse.png");
    mTextureHolder.load(Textures::ID::Lion, "assets/Textures/Lion.png");

    mTextureHolder.load(Textures::ID::Rock, "assets/Textures/Rock.png");
    mTextureHolder.load(Textures::ID::Tree, "assets/Textures/Tree.png");

    mTextureHolder.load(Textures::ID::Log, "assets/Textures/Log.png");

    mTextureHolder.load(
        Textures::ID::RailwaySignal, "assets/Textures/RailwaySignal.png"
    );
    mTextureHolder.load(
        Textures::ID::TrafficLight, "assets/Textures/TrafficLight.png"
    );

    mTextureHolder.load(
        Textures::ID::ObstacleLane, "assets/Textures/ObstacleLane.png"
    );
    mTextureHolder.load(
        Textures::ID::RiverLane, "assets/Textures/RiverLane.png"
    );
    mTextureHolder.load(
        Textures::ID::TrainLane, "assets/Textures/TrainLane.png"
    );
    mTextureHolder.load(
        Textures::ID::VehicleLane, "assets/Textures/VehicleLane.png"
    );

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

void Program::loadSettings() {
    std::ifstream fin("data/Settings.txt");

    if (!fin.good()) {
        fin.close();
        return;
    }

    fin.close();
}

void Program::registerStates() {
    mStateStack.registerState<TitleState>(States::ID::Title);
    mStateStack.registerState<MenuState>(States::ID::Menu);
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
