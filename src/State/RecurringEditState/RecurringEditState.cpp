#include "RecurringEditState.hpp"
#include "../WarningState/WarningState.hpp"
#include <iostream>

int RecurringEditState::sIndex = -1;
DynamicArray<RecurringTransaction> *RecurringEditState::sData = nullptr;

void RecurringEditState::setPayload(int index, DynamicArray<RecurringTransaction> *data)
{
    sIndex = index;
    sData = data;
}
RecurringEditState::RecurringEditState(StateStack &stack, Context context)
    : State(stack, context)
{
    mIndex = sIndex;
    mData = sData;
    sIndex = -1;

    sf::Vector2f center(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 2.f);

    const float startY = center.y - 180.f;
    const float gapY = 45.f;
    const float startX = center.x - 300.f;

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

    addField("Description:", startY + gapY * 2);
    mDescBox = addBox(startY + gapY * 2);

    addField("Day:", startY + gapY * 3);
    mDayBox = addBox(startY + gapY * 3);

    addField("Start Date:", startY + gapY * 4);
    mStartBox = addBox(startY + gapY * 4);

    addField("End Date:", startY + gapY * 5);
    mEndBox = addBox(startY + gapY * 5);

    // Prefill
    if (mData && mIndex >= 0)
    {
        auto &t = (*mData)[mIndex];
        mAmountBox->setText(std::to_string(t.amount));
        mWalletBox->setText(t.walletId);
        mDescBox->setText(t.description);
        mDayBox->setText(std::to_string(t.day));
        mStartBox->setText(t.startDate);
        mEndBox->setText(t.endDate);
    }

    auto saveBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Save");
    saveBtn->setPosition(center.x - 110.f, startY + gapY * 6 + 50.f);
    saveBtn->setCallback([this]()
                         { save(); });

    auto cancelBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Cancel");
    cancelBtn->setPosition(center.x + 110.f, startY + gapY * 6 + 50.f);
    cancelBtn->setCallback([this]()
                           { requestStackPop(); });

    mGUIContainer.addComponent(saveBtn);
    mGUIContainer.addComponent(cancelBtn);
}

void RecurringEditState::draw()
{
    auto &window = *getContext().window;

    window.setView(window.getDefaultView());

    sf::RectangleShape dim(window.getView().getSize());
    dim.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(dim);

    window.draw(mGUIContainer);
}

bool RecurringEditState::update(sf::Time deltaTime)
{
    return true;
}

bool RecurringEditState::handleEvent(const sf::Event &event)
{
    // Delegate event handling to the container
    // This handles typing in InputBox and clicking Buttons
    mGUIContainer.handleEvent(event, *getContext().window);

    // Optional: Allow 'Enter' key to save
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        save();
        requestStackPop();
    }

    return false;
}
void RecurringEditState::save()
{
    if (!mData || mIndex < 0)
        return;

    // ---- VALIDATION ----
    if (mAmountBox->getText().empty() ||
        mWalletBox->getText().empty() ||
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
    requestStackPop();
}

bool RecurringEditState::validateDateFormat(const std::string &dateStr)
{
    // Simple validation for YYYY-MM-DD format
    if (dateStr.length() != 10 ||
        dateStr[4] != '-' || dateStr[7] != '-')
    {
        return false;
    }

    for (size_t i = 0; i < dateStr.length(); ++i)
    {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(dateStr[i]))
            return false;
    }

    return true;
}
bool RecurringEditState::isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool RecurringEditState::isValidDate(int year, int month, int day)
{
    if (year < 1900 || year > 2100)
        return false;

    if (month < 1 || month > 12)
        return false;

    if (day < 1)
        return false;

    int daysInMonth[] = {
        31, // Jan
        28, // Feb
        31, // Mar
        30, // Apr
        31, // May
        30, // Jun
        31, // Jul
        31, // Aug
        30, // Sep
        31, // Oct
        30, // Nov
        31  // Dec
    };

    if (month == 2 && isLeapYear(year))
        return day <= 29;

    return day <= daysInMonth[month - 1];
}

bool RecurringEditState::isValidDateString(const std::string &date)
{
    if (date.size() != 10)
        return false;

    if (date[4] != '-' || date[7] != '-')
        return false;

    int year, month, day;

    try
    {
        year = std::stoi(date.substr(0, 4));
        month = std::stoi(date.substr(5, 2));
        day = std::stoi(date.substr(8, 2));
    }
    catch (...)
    {
        return false;
    }

    return isValidDate(year, month, day);
}