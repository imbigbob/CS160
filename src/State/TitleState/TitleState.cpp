#include "TitleState.hpp"

#include "../../Utility/Utility.hpp"

TitleState::TitleState(StateStack& stack, Context context)
    : State(stack, context),
      mShowText(true),
      mText(
          "Press any key to start", context.fontHolder->get(Fonts::ID::VTV323),
          30
      ) {
    sf::Vector2f windowSize(context.window->getSize());

    mBackgroundSprite.setTexture(
        context.textureHolder->get(Textures::ID::TitleBackground)
    );
    mBackgroundSprite.setScale(
        windowSize.x / mBackgroundSprite.getLocalBounds().width,
        windowSize.y / mBackgroundSprite.getLocalBounds().height
    );

    centerOrigin(mText);
    mText.setPosition(windowSize.x / 2.f, windowSize.y / 2.f + 120.f);
}

void TitleState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);

    if (mShowText) {
        window.draw(mText);
    }
}

bool TitleState::update(sf::Time deltaTime) {
    mTextEffectTime += deltaTime;

    if (mTextEffectTime >= sf::seconds(0.5f)) {
        mShowText = !mShowText;
        mTextEffectTime = sf::Time::Zero;
    }

    return true;
}

bool TitleState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyReleased ||
        event.type == sf::Event::MouseButtonReleased) {
        requestStackPop();
        requestStackPush(States::ID::Menu);
    }

    return true;
}
