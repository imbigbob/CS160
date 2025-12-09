#ifndef _RECURRING_TRANSACTION_STATE_HPP
#define _RECURRING_TRANSACTION_STATE_HPP

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../State.hpp"

class RecurringTransactionState : public State {
   public:
    RecurringTransactionState(StateStack& stack, Context context);

    bool handleEvent(const sf::Event& event) override;
    bool update(sf::Time deltaTime) override;
    void draw() override;

   private:
    sf::Sprite mBackgroundSprite;

    GUI::Container mGUIContainer;
};

#endif
