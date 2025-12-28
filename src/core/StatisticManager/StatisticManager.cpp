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
    return incomeManager.anualTransactionOverview(year);
}
double StatisticManager::annualExpenseOverview(DynamicArray<int> year)
{
    return expenseManager.anualTransactionOverview(year);
}

double StatisticManager::annualIncomeBreakdownByType(
    DynamicArray<int> year, const std::string &type)
{
    return incomeManager.transactionBreakdownByType(year, type);
}

double StatisticManager::annualExpenseBreakdownByType(
    DynamicArray<int> year, const std::string &type)
{
    return expenseManager.transactionBreakdownByType(year, type);
}
