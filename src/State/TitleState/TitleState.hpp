#ifndef TITLE_STATE_HPP
#define TITLE_STATE_HPP

#include "../State.hpp"

class TitleState : public State {
   public:
    TitleState(StateStack& stack, Context context);

    bool handleEvent(const sf::Event& event) override;
    bool update(sf::Time deltaTime) override;
    void draw() override;

   private:
    sf::Sprite mBackgroundSprite;
    sf::Text mText;
    sf::Time mTextEffectTime;
    bool mShowText;
};

#endif
