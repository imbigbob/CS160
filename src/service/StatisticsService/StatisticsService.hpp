#include <string>

#include "core/TransactionManager/TransactionManager.hpp"
class StatisticsService {
   public:
    double totalIncomeInRange(
        TransactionManager&, std::string from, std::string to
    );
    double totalExpenseInRange(
        TransactionManager&, std::string from, std::string to
    );

    double walletBasedIncome(TransactionManager&, int walletId, ...);
    double walletBasedExpense(TransactionManager&, int walletId, ...);

    void yearSummary(TransactionManager&, int year, ...);
    void incomeBreakdownBySource(TransactionManager&, int year);
    void expenseBreakdownByCategory(TransactionManager&, int year);
};
