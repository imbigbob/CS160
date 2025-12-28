#ifndef RECURRING_ADD_STATE_HPP
#define RECURRING_ADD_STATE_HPP

#include "../State.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class RecurringAddState : public State {
public:
    RecurringAddState(StateStack& stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time dt) override;
    virtual bool handleEvent(const sf::Event& event) override;

private:
    void handleInput(sf::Uint32 unicode);
    void saveRecurring();

private:
    sf::Sprite mBackground;
    sf::Sprite mBackButton;
    sf::Text mTitle;

    // Toggle Type
    int mType; // 0 = Expense, 1 = Income
    sf::Text mTypeText;
    sf::Sprite mTypeButton; // Changed to Sprite for Texture

    // Input Fields
    struct InputField {
        sf::Text label;
        sf::Text value;
        sf::RectangleShape box;
        std::string stringValue;
        bool isSelected;
    };

    // 0: Name, 1: Amount, 2: StartDate, 3: EndDate, 4: WalletID
    std::vector<InputField> mInputs;
    int mSelectedInputIndex;

    // Save Button
    sf::Sprite mSaveBtn;    // Changed to Sprite for Texture
    sf::Text mSaveBtnText;
};

#endif