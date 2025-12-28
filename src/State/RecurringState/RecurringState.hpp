#ifndef RECURRING_STATE_HPP
#define RECURRING_STATE_HPP

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class RecurringState : public State {
public:
    RecurringState(StateStack& stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time dt) override;
    virtual bool handleEvent(const sf::Event& event) override;

private:
    void loadRecurringData();

private:
    sf::Sprite mBackground;
    
    struct RecurringItem {
        sf::Text displayText;
        sf::RectangleShape background;
        int id;     // Index in the JSON array
        int type;   // 0 = Expense, 1 = Income
        sf::FloatRect bounds;
    };

    std::vector<RecurringItem> mItems;
    sf::Text mTitle;

    // Add Button
    sf::RectangleShape mAddButtonBg;
    sf::Text mAddButtonText;
    
    // Back Button
    sf::Sprite mBackButton;
};

#endif