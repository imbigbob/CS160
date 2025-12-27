#ifndef _RECURRING_EDIT_STATE_HPP
#define _RECURRING_EDIT_STATE_HPP

#include <memory>
#include <vector>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../GUI/InputBox/InputBox.hpp"
#include "../../core/RecurringManager/RecurringManager.hpp"
#include "../../core/TypeManager/TypeManager.hpp"

#include "../State.hpp"
class RecurringEditState : public State
{
public:
    static void setPayload(int index, DynamicArray<RecurringTransaction> *data);

    RecurringEditState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time) override;
    virtual bool handleEvent(const sf::Event &) override;

private:
    void save();
    bool validateDateFormat(const std::string &dateStr);
    bool isValidDateString(const std::string &dateStr);
    bool isLeapYear(int year);
    bool isValidDate(int year, int month, int day);

private:
    static int sIndex;
    static DynamicArray<RecurringTransaction> *sData;
    TypeManager incomeTypeManager{"IncomeTypes"};
    TypeManager expenseTypeManager{"ExpenseTypes"};
    TypeManager walletTypeManager{"WalletTypes"};
    GUI::Container mGUIContainer;

    int mIndex;
    DynamicArray<RecurringTransaction> *mData;

    std::shared_ptr<GUI::InputBox> mAmountBox;
    std::shared_ptr<GUI::InputBox> mWalletBox;
    std::shared_ptr<GUI::InputBox> mTypeBox;
    std::shared_ptr<GUI::InputBox> mDescBox;
    std::shared_ptr<GUI::InputBox> mDayBox;
    std::shared_ptr<GUI::InputBox> mStartBox;
    std::shared_ptr<GUI::InputBox> mEndBox;
};

#endif