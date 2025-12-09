#include <string>

#include "core/TransactionManager/TransactionManager.hpp"
class StatisticsService {
   public:
    double totalIncomeInRange(
        TransactionManager& tm, std::string from, std::string to
    );
    double totalExpenseInRange(
        TransactionManager& tm, std::string from, std::string to
    );

    double walletBasedIncome(TransactionManager& tm, int walletId);
    double walletBasedExpense(TransactionManager& tm, int walletId);
    void yearSummary(TransactionManager& tm, int year);
    void incomeBreakdownBySource(TransactionManager& tm, int year);
    void expenseBreakdownByCategory(TransactionManager& tm, int year);
};
