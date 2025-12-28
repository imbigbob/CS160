#include <ctime>
#include <string>

#include "core/DynamicArray/DynamicArray.hpp"
#include "core/TransactionManager/TransactionManager.hpp"
#include "model/Transaction/Transaction.hpp"
struct RecurringTransaction {
    Transaction baseTransaction();
    std::string startDate;
    std::string endDate;  // "" means infinite
};

class RecurringManager {
   private:
    DynamicArray<RecurringTransaction> Incomes;
    DynamicArray<RecurringTransaction> Expenses;

   private:
    void updateDB();
    bool checkTransactionExists(
        const RecurringTransaction& rt, const TransactionManager& tm
    );

   public:
    // RecurringManager();
    void addRule(const RecurringTransaction&);
    void applyForCurrentMonth(TransactionManager& tm);
    void updateRecurringTransactions(TransactionManager& tm);
    DynamicArray<RecurringTransaction>& getIncomes();
    DynamicArray<RecurringTransaction>& getExpenses();
    DynamicArray<RecurringTransaction>& getAll();
};

std::string getCurrentDate();
int getCurrentDay();