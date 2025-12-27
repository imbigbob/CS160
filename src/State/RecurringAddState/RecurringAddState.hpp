#ifndef _RECURRING_ADD_STATE_HPP
#define _RECURRING_ADD_STATE_HPP

#include <memory>
#include <vector>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../GUI/InputBox/InputBox.hpp"
#include "../../core/RecurringManager/RecurringManager.hpp"
#include "../State.hpp"
class RecurringAddState : public State
{
public:
    static void setPayload(DynamicArray<RecurringTransaction> *data);

    RecurringAddState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time) override;
    virtual bool handleEvent(const sf::Event &) override;

private:
    void save();
    bool isValidDateString(const std::string &dateStr);
    bool isLeapYear(int year);
    bool isValidDate(int year, int month, int day);

private:
    static int sIndex;
    static DynamicArray<RecurringTransaction> *sData;

    GUI::Container mGUIContainer;

    int mIndex;
    DynamicArray<RecurringTransaction> *mData;

    std::shared_ptr<GUI::InputBox> mAmountBox;
    std::shared_ptr<GUI::InputBox> mWalletBox;
    std::shared_ptr<GUI::InputBox> mDescBox;
    std::shared_ptr<GUI::InputBox> mDayBox;
    std::shared_ptr<GUI::InputBox> mStartBox;
    std::shared_ptr<GUI::InputBox> mEndBox;
};

#endif