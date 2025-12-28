#ifndef MASTER_MANAGEMENT_EDIT_STATE_HPP
#define MASTER_MANAGEMENT_EDIT_STATE_HPP

#include "../State.hpp"
#include "../GUI/Container/Container.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class MasterManagementEditState : public State {
public:
    MasterManagementEditState(StateStack& stack, Context context);

    virtual bool handleEvent(const sf::Event& event) override;
    virtual bool update(sf::Time deltaTime) override;
    virtual void draw() override;

private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;
    
    sf::Text mTitleText;
    
    sf::RectangleShape mInputBox;
    sf::Text mInputText;
    std::string mCurrentInput;
    int mTargetId;
    int mType;
};

#endif