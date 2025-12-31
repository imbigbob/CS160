#ifndef _MANAGEMENT_STATE_HPP
#define _MANAGEMENT_STATE_HPP

#include <memory>

#include "../../Utility/Utility.hpp"
#include "../../core/TypeManager/TypeManager.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../State.hpp"
#include "../../core/DynamicArray/DynamicArray.hpp"
class ManagementState : public State
{
public:
    ManagementState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time deltaTime) override;
    virtual bool handleEvent(const sf::Event &event) override;

private:
    void reloadTable();
    void updateScrollView();
    void handleEdit(int index);
    void handleDelete(int index);
    void handleAdd();

private:
    enum class Mode
    {
        Income,
        Expense,
        Wallet
    };

    sf::Sprite mBackgroundSprite;
    DynamicArray<sf::Sprite> mEditSprites;
    DynamicArray<sf::Sprite> mDeleteSprites;
    bool mNeedsRefresh = false;

    // Helper to handle the actual logic

    GUI::Container mGUIContainer;

    // Buttons tracked for text updates
    std::shared_ptr<GUI::Button> modeSwitchButton;
    std::shared_ptr<GUI::Button> addButton;

    // Data Management
    Mode currentMode;
    // Assuming you have these managers defined elsewhere or locally
    TypeManager incomeTypeManager{"IncomeTypes"};
    TypeManager expenseTypeManager{"ExpenseTypes"};
    TypeManager walletTypeManager{"WalletTypes"};

    sf::RectangleShape mTableHeader;
    DynamicArray<sf::Text> mHeaderTexts;

    // --- Table Content (Scrollable) ---
    DynamicArray<sf::RectangleShape> mRowRects; // Backgrounds for rows
    DynamicArray<sf::Text> mRowTexts;           // Text data for rows
    // --- Scrolling Mechanics ---
    sf::View mTableView;        // Camera for the list
    sf::FloatRect mTableBounds; // Visible area on screen
    float mScrollY;             // Current scroll position
    float mTotalContentHeight;  // Total height of all rows
};
#endif