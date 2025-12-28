#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "../GUI/Container/Container.hpp"
#include "../State.hpp"

class MenuState : public State {
   public:
    MenuState(StateStack& stack, Context context);

    bool handleEvent(const sf::Event& event) override;
    bool update(sf::Time deltaTime) override;
    void draw() override;

   private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;
};

#endif
