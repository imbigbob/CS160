#ifndef RECURRING_MANAGER_HPP
#define RECURRING_MANAGER_HPP

#include <ctime>
#include <string>

#include "core/DynamicArray/DynamicArray.hpp"
#include "core/IncomeManager/IncomeManager.hpp"
#include "core/ExpenseManager/ExpenseManager.hpp"

struct RecurringTransaction {
    // Transaction baseTransaction();

    // Similar to Income and Expense
    int type; // 1 is Income and 2 is Expense
    double amount;
    std::string categoryId;  
    std::string categoryName;
    std::string walletId;    
    std::string description; 

    int day; // applying day
    std::string startDate;
    std::string endDate;  // "" means infinite
    std::string lastAppliedYM; // YYYY-MM
};

class RecurringManager {
   private:
    DynamicArray<RecurringTransaction> incomeRules;
    DynamicArray<RecurringTransaction> expenseRules;

    std::string incomeFilepath;
    std::string expenseFilepath;

//    private:
    void updateDB(const std::string& filepath, DynamicArray<RecurringTransaction>& list);
    void loadFromDB(const std::string& filepath, DynamicArray<RecurringTransaction>& list, int type);

    std::string getCurrentYM();
//     bool checkTransactionExists(
//         const RecurringTransaction& rt, const TransactionManager& tm
//     );

   public:
    RecurringManager();
    void addRule(const RecurringTransaction& rule);
    void processRecurring(IncomeManager& im, ExpenseManager& em);
    // void applyForCurrentMonth(TransactionManager& tm);
    // void updateRecurringTransactions(TransactionManager& tm);
    DynamicArray<RecurringTransaction>& getIncomes();
    DynamicArray<RecurringTransaction>& getExpenses();
    // DynamicArray<RecurringTransaction>& getAll();
};

// std::string getCurrentDate();
// int getCurrentDay();

#endif