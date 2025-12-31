#ifndef TRANSACTION_EDIT_STATE_HPP
#define TRANSACTION_EDIT_STATE_HPP

#include <memory>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../GUI/InputBox/InputBox.hpp"
#include "../../core/TransactionManager/TransactionManager.hpp"
#include "../../core/TypeManager/TypeManager.hpp"
#include "../State.hpp"
class TransactionEditState : public State
{
public:
    static void setPayload(int index, int mode, TransactionManager *tManager);
    TransactionEditState(StateStack &stack, Context context);
    virtual void draw() override;
    virtual bool update(sf::Time) override;
    virtual bool handleEvent(const sf::Event &) override;

private:
    void save();

private:
    // --- NEW: Static variables to hold the data temporarily ---

    static int sPendingMode;
    static int sPendingIndex;
    static TransactionManager *sPendingTransactionManager;

    // Normal member variables for the instance
    int mEditingMode;
    int mIndex;
    TransactionManager *mTransactionManager;

    TypeManager walletTypeManager{"WalletTypes"};
    TypeManager typeManager{
        (mEditingMode == 0) ? "IncomeTypes"
                            : "ExpenseTypes"};
    GUI::Container mGUIContainer;

    std::shared_ptr<GUI::InputBox> mAmountBox;
    std::shared_ptr<GUI::InputBox> mWalletBox;
    std::shared_ptr<GUI::InputBox> mTypeBox;
    std::shared_ptr<GUI::InputBox> mDescBox;
};

#endif