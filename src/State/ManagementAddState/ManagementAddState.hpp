#ifndef _MANAGEMENT_ADD_STATE_HPP
#define _MANAGEMENT_ADD_STATE_HPP

#include <memory>
#include <vector>

#include "../../Utility/Utility.hpp"
#include "../../core/TypeManager/TypeManager.hpp"
#include "../../core/IncomeManager/IncomeManager.hpp" 
#include "../../core/ExpenseManager/ExpenseManager.hpp" 

#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../GUI/InputBox/InputBox.hpp"
#include "../State.hpp"

class ManagementAddState : public State
{
private:
    static int sPendingMode;
    static TypeManager *sPendingTypeManager;

    int mAddingMode;
    TypeManager *mTypeManager;

    GUI::Container mGUIContainer;

    // --- INPUT FIELDS ---
    std::shared_ptr<GUI::InputBox> mDateInput;    // New
    std::shared_ptr<GUI::InputBox> mSourceInput;  // Was mInputBox
    std::shared_ptr<GUI::InputBox> mAmountInput;  // Existing
    std::shared_ptr<GUI::InputBox> mWalletInput;  // New
    std::shared_ptr<GUI::InputBox> mDescInput;    // New

public:
    ManagementAddState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time deltaTime) override;
    virtual bool handleEvent(const sf::Event &event) override;
    static void setPayload(int mode, TypeManager *tManager);

private:
    void saveTypeData();
};
#endif