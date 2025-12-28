#ifndef MASTER_MANAGEMENT_STATE_HPP
#define MASTER_MANAGEMENT_STATE_HPP

#include "../State.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Button/Button.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class MasterManagementState : public State {
public:
    MasterManagementState(StateStack& stack, Context context);

    virtual bool handleEvent(const sf::Event& event) override;
    virtual bool update(sf::Time deltaTime) override;
    virtual void draw() override;

private:
    enum class Mode {
        Expense,
        Income,
        Wallet
    };

    void updateDisplayedText();
    void deleteItem(int id);

private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer; 
    std::shared_ptr<GUI::Container> mListContainer; 

    Mode mCurrentMode;
    sf::Text mTitleText;
    
    // --- FIX CRASH: Add a variable to track pending deletion ---
    int mPendingDeleteId; 

    struct ListItem {
        sf::RectangleShape background;
        sf::Text text;
        std::shared_ptr<GUI::Button> btnEdit;
        std::shared_ptr<GUI::Button> btnDelete;
    };

    std::vector<ListItem> mListItems; 
};

#endif