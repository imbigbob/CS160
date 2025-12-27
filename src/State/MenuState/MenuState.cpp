#include "MenuState.hpp"

#include <fstream>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
MenuState::MenuState(StateStack &stack, Context context)
    : State(stack, context)
{
    sf::Vector2f windowSize(context.window->getSize());
    sf::Texture &backgroundTexture =
        context.textureHolder->get(Textures::ID::MenuBackground);

    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y);

    auto transactionButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Transaction");
    transactionButton->setPosition(
        windowSize.x / 2.f, windowSize.y / 2.f - 50.f);
    transactionButton->setCallback([this]()
                                   { requestStackPush(States::ID::Transaction); });

    auto managementButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Management");
    managementButton->setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    managementButton->setCallback([this]()
                                  { requestStackPush(States::ID::Management); });

    auto recurringTransactionsButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Recurring");
    recurringTransactionsButton->setPosition(
        windowSize.x / 2.f, windowSize.y / 2.f + 50.f);

    recurringTransactionsButton->setCallback([this]()
                                             { requestStackPush(States::ID::Recurring); });

    auto statisticsButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Statistics");
    statisticsButton->setPosition(
        windowSize.x / 2.f, windowSize.y / 2.f + 100.f);
    statisticsButton->setCallback([this]()
                                  { requestStackPush(States::ID::Statistics); });

    auto exitButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Exit");
    exitButton->setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 150.f);
    exitButton->setCallback([this]()
                            { requestStackPop(); });

    mGUIContainer.addComponent(transactionButton);
    mGUIContainer.addComponent(managementButton);
    mGUIContainer.addComponent(recurringTransactionsButton);
    mGUIContainer.addComponent(statisticsButton);
    mGUIContainer.addComponent(exitButton);
}

bool MenuState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event, *getContext().window);
    return false;
}

bool MenuState::update(sf::Time deltaTime) { return true; }

void MenuState::draw()
{
    sf::RenderWindow &window = *getContext().window;
    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}
