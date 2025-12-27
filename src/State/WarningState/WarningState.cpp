#include "WarningState.hpp"
#include <iostream>
// static storage
std::string WarningState::sMessage;

void WarningState::setMessage(const std::string &msg)
{
    sMessage = msg;
}

WarningState::WarningState(StateStack &stack, Context context)
    : State(stack, context)
{
    sf::Vector2f center(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 2.f);

    // --- Warning Text ---
    mMessageText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mMessageText.setString(sMessage);
    mMessageText.setCharacterSize(28);
    mMessageText.setFillColor(sf::Color::Red);

    sf::FloatRect bounds = mMessageText.getLocalBounds();
    mMessageText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mMessageText.setPosition(center.x, center.y - 40.f);

    // --- Understand Button ---

    auto understandBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Understand");
    understandBtn->setPosition(center.x - 20.f, center.y + 20.f);
    std::cout << "WarningState: Understand button created\n";
    understandBtn->setCallback([this]()
                               { std::cout << "WarningState: Understand clicked\n";requestStackPop(); });

    mGUIContainer.addComponent(understandBtn);
}

void WarningState::draw()
{
    auto &window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape dim(window.getView().getSize());
    dim.setFillColor(sf::Color(255, 255, 255, 200));
    window.draw(dim);
    window.draw(mMessageText);
    window.draw(mGUIContainer);
}

bool WarningState::update(sf::Time)
{
    return true;
}

bool WarningState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event, *getContext().window);
    return false;
}
