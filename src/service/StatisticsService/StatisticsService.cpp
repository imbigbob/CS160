#include "StatisticsService.hpp"
double StatisticsService::totalIncomeInRange(
    TransactionManager& tm, std::string from, std::string to
) {
    return 0;
}

double StatisticsService::totalExpenseInRange(
    TransactionManager& tm, std::string from, std::string to
) {
    return 0;
}

double StatisticsService::walletBasedIncome(
    TransactionManager& tm, int walletId, ...
) {
    return 0;
}

double StatisticsService::walletBasedExpense(
    TransactionManager& tm, int walletId, ...
) {
    return 0;
}

void StatisticsService::yearSummary(TransactionManager& tm, int year, ...) {
    return;
}

void StatisticsService::incomeBreakdownBySource(
    TransactionManager& tm, int year
) {
    return;
}

void StatisticsService::expenseBreakdownByCategory(
    TransactionManager& tm, int year
) {
    return;
}
