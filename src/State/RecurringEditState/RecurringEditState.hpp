#ifndef RECURRING_EDIT_STATE_HPP
#define RECURRING_EDIT_STATE_HPP

#include "../State.hpp"
#include <SFML/Graphics.hpp>

class RecurringEditState : public State {
public:
    RecurringEditState(StateStack& stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time dt) override;
    virtual bool handleEvent(const sf::Event& event) override;

private:
    void deleteCurrentItem();

private:
    sf::Sprite mBackground;
    sf::Sprite mBackButton;
    sf::Text mTitle;

    // Background panel for info
    sf::RectangleShape mInfoPanel;
    sf::Text mInfoText;
    
    // Delete Button Components
    sf::Sprite mDeleteBtn;        // The button image (ButtonNormal.png)
    sf::Text mDeleteBtnText;      // The text "DELETE" on top of it

    int mId;
    int mType;
};

#endif