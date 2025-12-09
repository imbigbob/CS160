#include "StatisticsState.hpp"

StatisticsState::StatisticsState(StateStack& stack, Context context)
    : State(stack, context) {
    sf::Vector2f windowSize(context.window->getSize());
    sf::Texture& backgroundTexture =
        context.textureHolder->get(Textures::ID::MenuBackground);

    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y
    );

    auto backButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Back"
    );
    backButton->setPosition(250.f, 50.f);
    backButton->setCallback([this]() { requestStackPop(); });

    mGUIContainer.addComponent(backButton);
}

bool StatisticsState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);
    return false;
}

bool StatisticsState::update(sf::Time deltaTime) { return true; }

void StatisticsState::draw() {
    sf::RenderWindow& window = *getContext().window;

    window.draw(mBackgroundSprite);
    // window.draw(mFrameSprite);
    window.draw(mGUIContainer);
}
