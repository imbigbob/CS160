

#include <fstream>
#include <iostream>
#include <string>

#include "src/Program/Program.hpp"
#include "src/core/StatisticManager/StatisticManager.hpp"
int main()
{
    try
    {
        Program program;
        program.run();
    }
    catch (std::exception &e)
    {
        std::cout << "EXCEPTION bug: " << e.what() << '\n';
    }
    // StatisticManager statisticManager;
    // std::cout << statisticManager.getTotalIncome() << "\n";
    // std::cout << statisticManager.getTotalExpense() << "\n";
    // std::cout << statisticManager.getNetBalance() << "\n";
    // std::cout << statisticManager.getIncomeInTimeRange("2024-01-01", "2024-12-31") << "\n";
    // std::cout << statisticManager.getExpenseInTimeRange("2024-01-01", "2024-12-31") << "\n";
    // std::cout << statisticManager.getNetBalanceInTimeRange("2024-01-01", "2024-12-31") << "\n";
    // DynamicArray<int> years;
    // years.pushBack(2023);
    // years.pushBack(2024);
    // std::cout << statisticManager.annualIncomeOverview(years) << "\n";
    // std::cout << statisticManager.annualExpenseOverview(years) << "\n";
    // std::cout << statisticManager.annualIncomeBreakdownBySource(years, "Salary") << "\n";
    // std::cout << statisticManager.annualExpenseBreakdownByCategory(years, "Food") << "\n";

    return 0;
}
