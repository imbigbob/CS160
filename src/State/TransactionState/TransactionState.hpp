#ifndef TRANSACTION_STATE_HPP
#define TRANSACTION_STATE_HPP

#include "../../core/ExpenseManager/ExpenseManager.hpp"
#include "../../core/IncomeManager/IncomeManager.hpp"
#include "../GUI/Container/Container.hpp"
#include "../State.hpp"

class TransactionState : public State {
   public:
    TransactionState(StateStack& stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time deltaTime) override;
    virtual bool handleEvent(const sf::Event& event) override;

   private:
    void reloadTable();
    void updateScrollView();

   private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;

    // --- Table Headers ---
    sf::RectangleShape mTableHeader;
    std::vector<sf::Text> mHeaderTexts;

    // --- Table Content ---
    std::vector<sf::RectangleShape> mRowRects;
    std::vector<sf::Text> mRowTexts;

    // --- Scrolling ---
    sf::View mTableView;         
    sf::FloatRect mTableBounds;  
    float mScrollY;              
    float mTotalContentHeight;   

    enum class Mode { Income, Expense };
    Mode mMode = Mode::Income;
};
#endif