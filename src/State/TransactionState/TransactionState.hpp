#ifndef TRANSACTION_STATE_HPP
#define TRANSACTION_STATE_HPP

#include "../../core/ExpenseManager/ExpenseManager.hpp"
#include "../../core/IncomeManager/IncomeManager.hpp"
#include "../GUI/Container/Container.hpp"
#include "../State.hpp"
class TransactionState : public State {
   public:
    enum class Mode { Income, Expense };

   public:
    TransactionState(StateStack& stack, Context context);

    bool handleEvent(const sf::Event& event) override;
    bool update(sf::Time deltaTime) override;
    void draw() override;

   private:
    Mode mMode = Mode::Income;
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;
    sf::Sprite addSprite;
    sf::RectangleShape mTableHeader;
    std::vector<sf::Text> mHeaderTexts;

    std::vector<sf::RectangleShape> mRowRects;
    std::vector<sf::Text> mRowTexts;
    IncomeManager mIncomeManager;
    ExpenseManager mExpenseManager;

   private:
    void reloadTable();
};

#endif
