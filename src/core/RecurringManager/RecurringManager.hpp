#ifndef RECURRING_MANAGER_HPP
#define RECURRING_MANAGER_HPP

#include <ctime>
#include <string>

#include "core/DynamicArray/DynamicArray.hpp"
#include "core/IncomeManager/IncomeManager.hpp"
#include "core/ExpenseManager/ExpenseManager.hpp"
struct RecurringTransaction
{
    // Similar to Income and Expense
    int typeTransaction; // 1 is Income and 2 is Expense
    double amount;
    std::string categoryId;
    std::string type;
    std::string walletId;
    std::string description;

    int day; // applying day
    std::string startDate;
    std::string endDate;       // "" means infinite
    std::string lastAppliedYM; // YYYY-MM
};

class RecurringManager
{
private:
    DynamicArray<RecurringTransaction> incomeRules;
    DynamicArray<RecurringTransaction> expenseRules;
    IncomeManager incomeManagers;
    ExpenseManager expenseManagers;

    std::string incomeFilepath;
    std::string expenseFilepath;

private:
    void updateDB(const std::string &filepath, DynamicArray<RecurringTransaction> &list);
    void loadFromDB(const std::string &filepath, DynamicArray<RecurringTransaction> &list, int type);

    std::string getCurrentYM();

public:
    RecurringManager();
    void addRule(const RecurringTransaction &rule);
    void processRecurring();
    DynamicArray<RecurringTransaction> &getIncomes() { return incomeRules; };
    DynamicArray<RecurringTransaction> &getExpenses() { return expenseRules; };
};

#endif