#ifndef TRANSACTION_ADD_STATE_HPP
#define TRANSACTION_ADD_STATE_HPP

#include <memory>
#include <vector>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../GUI/InputBox/InputBox.hpp"
#include "../State.hpp"
#include "../../core/TransactionManager/TransactionManager.hpp"
class TransactionAddState : public State
{

private:
    // --- NEW: Static variables to hold the data temporarily ---

    static int sPendingMode;
    static TransactionManager *sPendingTransactionManager;

    // Normal member variables for the instance
    int mAddingMode;

    GUI::Container mGUIContainer;

    TransactionManager *mTransactionManager;

    std::shared_ptr<GUI::InputBox> mAmountBox;
    std::shared_ptr<GUI::InputBox> mWalletBox;
    std::shared_ptr<GUI::InputBox> mTypeBox;
    std::shared_ptr<GUI::InputBox> mDescBox;

public:
    TransactionAddState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time deltaTime) override;
    virtual bool handleEvent(const sf::Event &event) override;
    static void setPayload(int mode, TransactionManager *tManager);

private:
    void save();
};
#endif