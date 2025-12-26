#include "StatisticsState.hpp"
#include <sstream>
#include <iomanip>

// Helper to format doubles to string with 2 decimal places
std::string formatMoney(double amount)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << amount;
    return ss.str();
}
StatisticsState::StatisticsState(StateStack &stack, Context context)
    : State(stack, context)
{
    sf::Vector2f windowSize(context.window->getSize());
    sf::Texture &backgroundTexture =
        context.textureHolder->get(Textures::ID::StatBackground);

    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y);

    auto backButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Back");
    backButton->setPosition(250.f, 50.f);
    backButton->setCallback([this]()
                            { requestStackPop(); });

    float inputX = windowSize.x / 2 - 400.f;
    float currentY = windowSize.y / 2 - 200.f;
    float gapY = 60.f;

    auto addInput = [&](std::string labelText, std::shared_ptr<GUI::InputBox> &boxPtr, std::string placeholder)
    {
        // Label
        auto label = std::make_shared<GUI::Label>(labelText, *context.fontHolder);
        label->setPosition(inputX, currentY);
        label->setTextColor(sf::Color::Black);
        mGUIContainer.addComponent(label);

        // Input Box
        boxPtr = std::make_shared<GUI::InputBox>(*context.fontHolder);
        boxPtr->setPosition(inputX, currentY + 25.f);
        boxPtr->setText(placeholder);

        mGUIContainer.addComponent(boxPtr);

        currentY += gapY;
    };
    mTableBackground1.setPosition(inputX - 20.f, currentY - 20.f);
    mTableBackground1.setSize(sf::Vector2f(300.f, 400.f));

    // Color: White with slight transparency (230 alpha) so it looks nice
    mTableBackground1.setFillColor(sf::Color(255, 255, 255, 230));

    addInput("Start Date (YYYY-MM-DD)", mStartDateInput, "2024-01-01");
    addInput("End Date (YYYY-MM-DD)", mEndDateInput, "2024-12-31");
    addInput("Years (e.g. 2024, 2025)", mYearInput, "2024");
    addInput("Source Name", mSourceInput, "Salary");
    addInput("Category Name", mCategoryInput, "Food");

    // "Calculate" Button
    auto calcBtn = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Update Stats");
    calcBtn->setPosition(inputX + 100.f, currentY + 25.f);
    calcBtn->setCallback([this]()
                         { updateStatistics(); });

    // --- RIGHT COLUMN SETUP ---
    float tableLabelX = windowSize.x / 2 + 50.f;
    float tableValueX = windowSize.x / 2 + 250.f;
    float tableStartY = windowSize.y / 2 - 200.f; // Renamed for clarity
    float currentTableY = tableStartY;
    float rowHeight = 40.f;

    auto addTableRow = [&](std::string key, std::string title)
    {
        // 1. Title Label (Left side of table)
        auto titleLbl = std::make_shared<GUI::Label>(title, *context.fontHolder);
        titleLbl->setPosition(tableLabelX, tableStartY);
        titleLbl->setTextColor(sf::Color::Black);
        mGUIContainer.addComponent(titleLbl);

        // 2. Value Label (Right side of table)
        auto valLbl = std::make_shared<GUI::Label>("---", *context.fontHolder);
        valLbl->setPosition(tableValueX, tableStartY);
        valLbl->setTextColor(sf::Color::Black);
        mGUIContainer.addComponent(valLbl);

        // Store pointer to update later
        mValueLabels[key] = valLbl;

        tableStartY += rowHeight;
    };
    mTableBackground2.setPosition(tableLabelX - 20.f, tableStartY - 20.f);
    mTableBackground2.setSize(sf::Vector2f(450.f, 500.f));

    // Color: White with slight transparency (230 alpha) so it looks nice
    mTableBackground2.setFillColor(sf::Color(255, 255, 255, 230));

    // Optional: Add a border
    mTableBackground2.setOutlineThickness(1.f);
    mTableBackground2.setOutlineColor(sf::Color::Black);
    // -- GENERAL STATS --
    addTableRow("total_inc", "Total Income:");
    addTableRow("total_exp", "Total Expense:");
    addTableRow("net_bal", "Net Balance:");

    tableStartY += 20.f; // Spacer

    // -- TIME RANGE STATS --
    addTableRow("range_inc", "Income (Range):");
    addTableRow("range_exp", "Expense (Range):");
    addTableRow("range_net", "Net (Range):");

    tableStartY += 20.f; // Spacer

    // -- ANNUAL / BREAKDOWN --
    addTableRow("annual_inc", "Annual Income:");
    addTableRow("annual_exp", "Annual Expense:");
    addTableRow("src_break", "Source Breakdown:");
    addTableRow("cat_break", "Category Breakdown:");
    mGUIContainer.addComponent(calcBtn);

    mGUIContainer.addComponent(backButton);
    updateStatistics(); // Initial calculation
}

bool StatisticsState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event, *getContext().window);
    return false;
}

bool StatisticsState::update(sf::Time deltaTime) { return true; }

void StatisticsState::draw()
{
    sf::RenderWindow &window = *getContext().window;

    window.draw(mBackgroundSprite);
    window.draw(mTableBackground1);
    window.draw(mTableBackground2);
    window.draw(mGUIContainer);
}
void StatisticsState::updateStatistics()
{

    // 2. Get Inputs
    std::string start = mStartDateInput->getText();
    std::string end = mEndDateInput->getText();
    std::string src = mSourceInput->getText();
    std::string cat = mCategoryInput->getText();

    DynamicArray<int> years = parseYears(mYearInput->getText());

    // 3. Update Labels (The Table)

    // General
    mValueLabels["total_inc"]->setText(formatMoney(mStatisticManager.getTotalIncome()));
    mValueLabels["total_exp"]->setText(formatMoney(mStatisticManager.getTotalExpense()));
    mValueLabels["net_bal"]->setText(formatMoney(mStatisticManager.getNetBalance()));

    // Range
    mValueLabels["range_inc"]->setText(formatMoney(mStatisticManager.getIncomeInTimeRange(start, end)));
    mValueLabels["range_exp"]->setText(formatMoney(mStatisticManager.getExpenseInTimeRange(start, end)));
    mValueLabels["range_net"]->setText(formatMoney(mStatisticManager.getNetBalanceInTimeRange(start, end)));

    // Annual / Breakdown
    mValueLabels["annual_inc"]->setText(formatMoney(mStatisticManager.annualIncomeOverview(years)));
    mValueLabels["annual_exp"]->setText(formatMoney(mStatisticManager.annualExpenseOverview(years)));
    mValueLabels["src_break"]->setText(formatMoney(mStatisticManager.annualIncomeBreakdownBySource(years, src)));
    mValueLabels["cat_break"]->setText(formatMoney(mStatisticManager.annualExpenseBreakdownByCategory(years, cat)));
}

DynamicArray<int> StatisticsState::parseYears(const std::string &text)
{
    DynamicArray<int> result;
    std::stringstream ss(text);
    std::string segment;

    // Split by comma
    while (std::getline(ss, segment, ','))
    {
        try
        {
            // Trim spaces if needed, but std::stoi usually handles leading spaces
            result.pushBack(std::stoi(segment));
        }
        catch (...)
        {
            // Ignore invalid numbers
        }
    }
    return result;
}