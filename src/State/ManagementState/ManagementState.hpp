#ifndef _MANAGEMENT_STATE_HPP
#define _MANAGEMENT_STATE_HPP

#include <memory>
#include <vector>

#include "../../Utility/Utility.hpp"
#include "../../core/TypeManager/TypeManager.hpp"

// [Integration] Include Income headers to support the new system
#include "../../core/IncomeManager/IncomeManager.hpp"
#include "../../model/Income/Income.hpp"

#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../State.hpp"

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
    std::vector<sf::Sprite> mEditSprites;
    std::vector<sf::Sprite> mDeleteSprites;
    bool mNeedsRefresh = false;

    GUI::Container mGUIContainer;

    std::shared_ptr<GUI::Button> modeSwitchButton;
    std::shared_ptr<GUI::Button> addButton;

    Mode currentMode;

    // Managers (Legacy for Expense/Wallet)
    TypeManager incomeTypeManager{"IncomeTypes"}; 
    TypeManager expenseTypeManager{"ExpenseTypes"};
    TypeManager walletTypeManager{"WalletTypes"};

    sf::RectangleShape mTableHeader;
    std::vector<sf::Text> mHeaderTexts;

    // Table Content
    std::vector<sf::RectangleShape> mRowRects; 
    std::vector<sf::Text> mRowTexts;           

    // Scrolling
    sf::View mTableView;        
    sf::FloatRect mTableBounds; 
    float mScrollY;             
    float mTotalContentHeight;  
};
#endif