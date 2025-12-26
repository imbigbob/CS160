#include "StatisticManager.hpp"
#include "core/DynamicArray/DynamicArray.hpp"
#include <ctime>
std::string StatisticManager::setDate()
{
    // Get current date as "YYYY-MM-DD"
    std::time_t t = std::time(nullptr);
    std::tm *tmPtr = std::localtime(&t);

    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", tmPtr);

    return std::string(buffer);
}

double StatisticManager::getTotalIncome()
{

    return incomeManager.getTotalBalance();
}

double StatisticManager::getTotalExpense() { return expenseManager.getTotalBalance(); }
double StatisticManager::getNetBalance() { return incomeManager.getTotalBalance() - expenseManager.getTotalBalance(); }
double StatisticManager::getIncomeInTimeRange(
    const std::string &start, const std::string &end)
{
    return incomeManager.getBalanceInTimeRange(start, end);
}

double StatisticManager::getExpenseInTimeRange(
    const std::string &start, const std::string &end)
{
    return expenseManager.getBalanceInTimeRange(start, end);
}

double StatisticManager::getNetBalanceInTimeRange(
    const std::string &start, const std::string &end)
{
    return incomeManager.getBalanceInTimeRange(start, end) - expenseManager.getBalanceInTimeRange(start, end);
}

double StatisticManager::annualIncomeOverview(DynamicArray<int> year)
{
    return incomeManager.anualIncomeOverview(year);
}
double StatisticManager::annualExpenseOverview(DynamicArray<int> year)
{
    return expenseManager.anualExpenseOverview(year);
}

double StatisticManager::annualIncomeBreakdownBySource(
    DynamicArray<int> year, const std::string &sourceName)
{
    return incomeManager.incomeBreakdownBySource(year, sourceName);
}

double StatisticManager::annualExpenseBreakdownByCategory(
    DynamicArray<int> year, const std::string &categoryName)
{
    return expenseManager.expenseBreakdownByCategory(year, categoryName);
}
