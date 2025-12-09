#ifndef TRANSACTION_STATE_HPP
#define TRANSACTION_STATE_HPP

#include "../GUI/Container/Container.hpp"
#include "../State.hpp"

class TransactionState : public State {
   public:
    TransactionState(StateStack& stack, Context context);

    bool handleEvent(const sf::Event& event) override;
    bool update(sf::Time deltaTime) override;
    void draw() override;

   private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;
};

#endif
