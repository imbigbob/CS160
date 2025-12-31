#ifndef TRANSACTION_STATE_HPP
#define TRANSACTION_STATE_HPP

#include "../../core/TransactionManager/TransactionManager.hpp"
#include "../GUI/Container/Container.hpp"
#include "../State.hpp"
#include "../../core/DynamicArray/DynamicArray.hpp"

#include "../../core/TypeManager/TypeManager.hpp"
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
    void handleEdit(int index);
    void handleDelete(int index);

private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;

    DynamicArray<sf::Sprite> mEditSprites;
    DynamicArray<sf::Sprite> mDeleteSprites;

    // --- Table Headers (Static) ---
    sf::RectangleShape mTableHeader;
    DynamicArray<sf::Text> mHeaderTexts;
    TransactionManager mIncomeManager{"data/IncomesTransaction.json"};
    TransactionManager mExpenseManager{"data/ExpensesTransaction.json"};
    TypeManager walletTypeManager{"WalletTypes"};
    TypeManager incomeTypeManager{"IncomeTypes"};
    TypeManager expenseTypeManager{"ExpenseTypes"};
    // --- Table Content (Scrollable) ---
    DynamicArray<sf::RectangleShape> mRowRects;
    DynamicArray<sf::Text> mRowTexts;

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
