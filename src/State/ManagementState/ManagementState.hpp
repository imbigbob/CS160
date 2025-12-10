#ifndef _MANAGEMENT_STATE_HPP
#define _MANAGEMENT_STATE_HPP

#include "../../Utility/Utility.hpp"
#include "../../core/TypeManager/TypeManager.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../State.hpp"
class ManagementState : public State {
   public:
    enum class Mode { Income, Expense, Wallet };
    GUI::Button::Ptr modeSwitchButton;
    GUI::Button::Ptr addButton;

    // Scroll list
    std::vector<GUI::Label::Ptr> listItems;
    float listStartY = 150.f;
    float listItemHeight = 40.f;
    float scrollOffset = 0.f;  // current scroll value
    float maxScroll = 0.f;

   private:
    sf::Sprite mBackgroundSprite;

    GUI::Container mGUIContainer;
    TypeManager incomeTypeManager{"IncomeTypes"};
    TypeManager expenseTypeManager{"ExpenseTypes"};
    TypeManager walletTypeManager{"WalletTypes"};

    Mode currentMode = Mode::Income;

   public:
    ManagementState(StateStack& stack, Context context);

    bool handleEvent(const sf::Event& event) override;
    bool update(sf::Time deltaTime) override;
    void draw() override;

   private:
    void loadList();
};

#endif
