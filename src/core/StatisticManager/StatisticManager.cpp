#include "StatisticManager.hpp"
StatisticManager::StatisticManager(IncomeManager& im, ExpenseManager& em)
    : incomeManager(im), expenseManager(em) {}

double StatisticManager::getTotalIncome() { return 0.0; }

double StatisticManager::getTotalExpense() { return 0.0; }
double StatisticManager::getNetBalance() { return 0.0; }

double StatisticManager::getIncomeInTimeRange(
    const std::string& start, const std::string& end
) {
    return 0.0;
}

double StatisticManager::getExpenseInTimeRange(
    const std::string& start, const std::string& end
) {
    return 0.0;
}

double StatisticManager::getNetBalanceInTimeRange(
    const std::string& start, const std::string& end
) {
    return 0.0;
}

double StatisticManager::annualIncomeOverview(std::vector<int> year) {
    return 0.0;
}

double StatisticManager::annualExpenseOverview(std::vector<int> year) {
    return 0.0;
}

double StatisticManager::annualIncomeBreakdownBySource(
    std::vector<int> year, const std::string& sourceName
) {
    return 0.0;
}

double StatisticManager::annualExpenseBreakdownByCategory(
    std::vector<int> year, const std::string& categoryName
) {
    return 0.0;
}
