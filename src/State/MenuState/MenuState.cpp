#include "MenuState.hpp"

#include <fstream>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
MenuState::MenuState(StateStack& stack, Context context)
    : State(stack, context) {
    sf::Vector2f windowSize(context.window->getSize());
    sf::Texture& backgroundTexture =
        context.textureHolder->get(Textures::ID::MenuBackground);

    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y
    );

    auto newSingleplayerButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "New Singleplayer"
    );
    newSingleplayerButton->setPosition(
        windowSize.x / 2.f, windowSize.y / 2.f - 50.f
    );
    newSingleplayerButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::ID::SelectPlayer1Single);
    });

    auto loadSingleplayerButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Load Singleplayer"
    );
    loadSingleplayerButton->setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    loadSingleplayerButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::ID::LoadSingleGame);
    });

    if (!isSinglePlayerLoadable()) {
        loadSingleplayerButton->setSelectable(false);
    }

    auto newMutiplayerButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "New Mutiplayer"
    );
    newMutiplayerButton->setPosition(
        windowSize.x / 2.f, windowSize.y / 2.f + 50.f
    );
    newMutiplayerButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::ID::SelectPlayer1Multi);
    });

    auto loadMultiplayerButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Load Multiplayer"
    );
    loadMultiplayerButton->setPosition(
        windowSize.x / 2.f, windowSize.y / 2.f + 100.f
    );
    loadMultiplayerButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::ID::LoadMultiplayerGame);
    });

    if (!isMultiPlayerLoadable()) {
        loadMultiplayerButton->setSelectable(false);
    }

    auto settingButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Setting"
    );
    settingButton->setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 150.f);
    settingButton->setCallback([this]() {
        requestStackPush(States::ID::Settings);
    });

    auto highScoreButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "High Score"
    );
    highScoreButton->setPosition(
        windowSize.x / 2.f, windowSize.y / 2.f + 200.f
    );
    highScoreButton->setCallback([this]() {
        requestStackPush(States::ID::HighScore);
    });

    auto exitButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Exit"
    );
    exitButton->setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 250.f);
    exitButton->setCallback([this]() { requestStackPop(); });

    mGUIContainer.addComponent(newSingleplayerButton);
    mGUIContainer.addComponent(loadSingleplayerButton);
    mGUIContainer.addComponent(newMutiplayerButton);
    mGUIContainer.addComponent(loadMultiplayerButton);
    mGUIContainer.addComponent(settingButton);
    mGUIContainer.addComponent(highScoreButton);
    mGUIContainer.addComponent(exitButton);
}

bool MenuState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);
    return false;
}

bool MenuState::update(sf::Time deltaTime) { return true; }

void MenuState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}

bool MenuState::isSinglePlayerLoadable() {
    std::ifstream fin;
    int textureID;
    fin.open("data/Singleplayer.txt");

    if (!fin.good()) {
        fin.close();
        return false;
    }

    fin.close();
    fin.open("data/PlayerTexture.txt");

    if (!fin.good()) {
        fin.close();
        return false;
    }

    fin >> textureID;

    if (textureID == static_cast<int>(Textures::ID::None)) {
        return false;
    }

    fin.close();
    return true;
}

bool MenuState::isMultiPlayerLoadable() {
    std::ifstream fin;
    int textureID;
    fin.open("data/Multiplayer.txt");

    if (!fin.good()) {
        fin.close();
        return false;
    }

    fin.close();
    fin.open("data/PlayerTexture.txt");

    if (!fin.good()) {
        fin.close();
        return false;
    }

    fin >> textureID;

    for (int i = 0; i < 2; ++i) {
        fin >> textureID;

        if (textureID == static_cast<int>(Textures::ID::None)) {
            return false;
        }
    }

    fin.close();
    return true;
}
