#ifndef RECURRING_MANAGER_HPP
#define RECURRING_MANAGER_HPP

#include <string>
#include "../IncomeManager/IncomeManager.hpp"
#include "../ExpenseManager/ExpenseManager.hpp"

class RecurringManager
{
public:
    // Checks JSON configurations and adds transactions if they are due for the current month
    void processDailyCheck(IncomeManager &incomeMgr, ExpenseManager &expenseMgr);

    // Manually add a new Recurring Income to the JSON file
    void addRecurringIncome(const std::string &sourceName, double amount, 
                            const std::string &startDate, const std::string &endDate, 
                            const std::string &description, const std::string &walletId,
                            IncomeManager &incomeMgr, ExpenseManager &expenseMgr);

    // Manually add a new Recurring Expense to the JSON file
    void addRecurringExpense(const std::string &categoryName, double amount, 
                             const std::string &startDate, const std::string &endDate, 
                             const std::string &description, const std::string &walletId, 
                             const std::string &walletName,
                             IncomeManager &incomeMgr, ExpenseManager &expenseMgr);

private:
    // Helper to get today's date in "YYYY-MM-DD"
    std::string getCurrentDate();
    
    // Helper to get the current month prefix "YYYY-MM"
    std::string getCurrentMonthYear();

    // Helper to check if an income transaction already exists for this month
    bool isTransactionLoggedForMonth(DynamicArray<Income> &list,
                                     const std::string &sourceName,
                                     double amount,
                                     const std::string &currentMonthYear);

    // Helper to check if an expense transaction already exists for this month
    bool isTransactionLoggedForMonth(DynamicArray<Expense> &list,
                                     const std::string &categoryName,
                                     double amount,
                                     const std::string &currentMonthYear);
};

#endif