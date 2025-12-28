#ifndef TRANSACTION_STATE_HPP
#define TRANSACTION_STATE_HPP

#include "../../core/ExpenseManager/ExpenseManager.hpp"
#include "../../core/IncomeManager/IncomeManager.hpp"
#include "../GUI/Container/Container.hpp"
#include "../State.hpp"

class TransactionState : public State
{
public:
    TransactionState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time deltaTime) override;
    virtual bool handleEvent(const sf::Event &event) override;

private:
    void reloadTable();
    void updateScrollView(); // Helper to keep scroll in bounds
    void handleAdd();

private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;

    // --- Table Headers (Static) ---
    sf::RectangleShape mTableHeader;
    std::vector<sf::Text> mHeaderTexts;
    IncomeManager mIncomeManager;
    ExpenseManager mExpenseManager;

    // --- Table Content (Scrollable) ---
    std::vector<sf::RectangleShape> mRowRects;
    std::vector<sf::Text> mRowTexts;

    // --- Scrolling Mechanics ---
    sf::View mTableView;        // The camera for the list
    sf::FloatRect mTableBounds; // The visible area on screen
    float mScrollY;             // Current scroll position
    float mTotalContentHeight;  // Total height of all rows combined

    // Mode tracking
    enum class Mode
    {
        Income,
        Expense
    };
    Mode mMode = Mode::Income;
};
#endif
