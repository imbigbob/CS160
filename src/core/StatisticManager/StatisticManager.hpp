#ifndef STATISTICMANAGER_HPP
#define STATISTICMANAGER_HPP

#include <string>
#include <vector>

#include "core/TransactionManager/TransactionManager.hpp"
#include "core/DynamicArray/DynamicArray.hpp"
#include <ctime>
class StatisticManager
{
private:
    TransactionManager incomeManager{"data/IncomesTransaction.json"};
    TransactionManager expenseManager{"data/ExpensesTransaction.json"};
    std::string startDate; // dd/mm/yyyy
    std::string endDate;   // dd/mm/yyyy

public:
    std::string setDate();
    StatisticManager()
    {
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
    double annualIncomeBreakdownByType(
        DynamicArray<int> year, const std::string &type);
    double annualExpenseBreakdownByType(
        DynamicArray<int> year, const std::string &type);
};
#endif