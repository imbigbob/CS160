#ifndef STATISTICMANAGER_HPP
#define STATISTICMANAGER_HPP

#include <string>
#include <vector>

#include "core/ExpenseManager/ExpenseManager.hpp"
#include "core/IncomeManager/IncomeManager.hpp"
#include "core/DynamicArray/DynamicArray.hpp"
#include <ctime>
class StatisticManager
{
private:
    IncomeManager incomeManager;
    ExpenseManager expenseManager;
    std::string startDate; // dd/mm/yyyy
    std::string endDate;   // dd/mm/yyyy

public:
    std::string setDate();
    StatisticManager()
    {
        incomeManager = IncomeManager();
        expenseManager = ExpenseManager();
        startDate = "01/01/2023";
        endDate = setDate();
    }; // assign the date open app or the first date in this month

    double getTotalIncome();

    double getTotalExpense();
    double getNetBalance();
    double getIncomeInTimeRange(
        const std::string &start, const std::string &end);
    double getExpenseInTimeRange(
        const std::string &start, const std::string &end);
    double getNetBalanceInTimeRange(
        const std::string &start, const std::string &end);
    double annualIncomeOverview(DynamicArray<int> year);
    double annualExpenseOverview(DynamicArray<int> year);
    double annualIncomeBreakdownBySource(
        DynamicArray<int> year, const std::string &sourceName);
    double annualExpenseBreakdownByCategory(
        DynamicArray<int> year, const std::string &categoryName);
};
#endif