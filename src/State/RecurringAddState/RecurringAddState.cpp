
#include "RecurringAddState.hpp"
#include "../WarningState/WarningState.hpp"
#include <cctype>
#include <iostream>

DynamicArray<RecurringTransaction> *RecurringAddState::sData = nullptr;
int RecurringAddState::sIndex = -1;

void RecurringAddState::setPayload(int index, DynamicArray<RecurringTransaction> *data)
{
    sIndex = index;
    sData = data;
}

RecurringAddState::RecurringAddState(StateStack &stack, Context context)
    : State(stack, context)
{
    mData = sData;
    mIndex = sIndex;

    sf::Vector2f center(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 2.f);

    const float startY = center.y - 180.f;
    const float gapY = 45.f;
    const float startX = center.x - 350.f;

    auto addField = [&](const std::string &label, float y)
    {
        auto l = std::make_shared<GUI::Label>(label, *context.fontHolder);
        l->setPosition(startX, y);
        mGUIContainer.addComponent(l);
    };

    auto addBox = [&](float y)
    {
        auto b = std::make_shared<GUI::InputBox>(*context.fontHolder);
        b->setPosition(startX + 140.f, y);
        b->setScale(1.5f, 1.5f);
        mGUIContainer.addComponent(b);
        return b;
    };

    addField("Amount:", startY);
    mAmountBox = addBox(startY);

    addField("Wallet ID:", startY + gapY);
    mWalletBox = addBox(startY + gapY);

    addField("Type ID:", startY + gapY * 2);
    mTypeBox = addBox(startY + gapY * 2);

    addField("Description:", startY + gapY * 3);
    mDescBox = addBox(startY + gapY * 3);

    addField("Day:", startY + gapY * 4);
    mDayBox = addBox(startY + gapY * 4);

    addField("Start Date:", startY + gapY * 5);
    mStartBox = addBox(startY + gapY * 5);

    addField("End Date:", startY + gapY * 6);
    mEndBox = addBox(startY + gapY * 6);

    auto saveBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Save");
    saveBtn->setPosition(center.x - 110.f, startY + gapY * 7 + 50.f);
    saveBtn->setCallback([this]()
                         { save(); });

    auto cancelBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Cancel");
    cancelBtn->setPosition(center.x + 110.f, startY + gapY * 7 + 50.f);
    cancelBtn->setCallback([this]()
                           { requestStackPop(); });

    mGUIContainer.addComponent(saveBtn);
    mGUIContainer.addComponent(cancelBtn);
}
void RecurringAddState::draw()
{
    auto &window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape dim(window.getView().getSize());
    dim.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(dim);

    window.draw(mGUIContainer);
}
bool RecurringAddState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event, *getContext().window);

    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter)
    {
        save();
    }

    return false;
}
void RecurringAddState::save()
{
    if (!mData)
        return;

    if (mAmountBox->getText().empty() ||
        mWalletBox->getText().empty() ||
        mTypeBox->getText().empty() ||
        mDescBox->getText().empty() ||
        mDayBox->getText().empty() ||
        mStartBox->getText().empty())
    {
        WarningState::setMessage("Please fill in all required fields!");
        requestStackPush(States::ID::Warning);
        return;
    }

    if (!isValidDateString(mStartBox->getText()))
    {
        WarningState::setMessage("Invalid Start Date (YYYY-MM-DD)");
        requestStackPush(States::ID::Warning);
        return;
    }

    if (!mEndBox->getText().empty() &&
        !isValidDateString(mEndBox->getText()))
    {
        WarningState::setMessage("Invalid End Date (YYYY-MM-DD)");
        requestStackPush(States::ID::Warning);
        return;
    }

    if (std::stof(mAmountBox->getText()) <= 0.f)
    {
        WarningState::setMessage("Amount must be positive!");
        requestStackPush(States::ID::Warning);
        return;
    }

    if (mIndex == 0)
    {
        std::cout << "Income Recurring Transaction Added" << std::endl;
        std::string incomeId = mTypeBox->getText();
        if (!incomeTypeManager.isTypeIdExist(incomeId))
        {
            WarningState::setMessage("Income Type ID does not exist!");
            requestStackPush(States::ID::Warning);
            return;
        }
    }
    else if (mIndex == 1)
    {
        std::cout << "Expense Recurring Transaction Added" << std::endl;
        std::string expenseId = mTypeBox->getText();
        if (!expenseTypeManager.isTypeIdExist(expenseId))
        {
            WarningState::setMessage("Expense Type ID does not exist!");
            requestStackPush(States::ID::Warning);
            return;
        }
    }
    else
    {
        WarningState::setMessage("Invalid Transaction Type!");
        requestStackPush(States::ID::Warning);
        return;
    }

    std::string walletId = mWalletBox->getText();
    if (!walletTypeManager.isTypeIdExist(walletId))
    {
        WarningState::setMessage("Wallet Type ID does not exist!");
        requestStackPush(States::ID::Warning);
        return;
    }

    RecurringTransaction t;
    t.amount = std::stof(mAmountBox->getText());
    t.walletId = mWalletBox->getText();

    t.description = mDescBox->getText();
    t.day = std::stoi(mDayBox->getText());
    t.startDate = mStartBox->getText();
    t.endDate = mEndBox->getText();

    mData->pushBack(t);

    requestStackPop();
}
bool RecurringAddState::isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool RecurringAddState::isValidDate(int y, int m, int d)
{
    if (y < 1900 || y > 2100)
        return false;
    if (m < 1 || m > 12)
        return false;

    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && isLeapYear(y))
        days[1] = 29;

    return d >= 1 && d <= days[m - 1];
}

bool RecurringAddState::isValidDateString(const std::string &date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    try
    {
        int y = std::stoi(date.substr(0, 4));
        int m = std::stoi(date.substr(5, 2));
        int d = std::stoi(date.substr(8, 2));
        return isValidDate(y, m, d);
    }
    catch (...)
    {
        return false;
    }
}

bool RecurringAddState::update(sf::Time)
{
    return true;
}