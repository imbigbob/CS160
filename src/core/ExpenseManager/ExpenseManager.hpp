#ifndef EXPENSE_MANAGER_HPP
#define EXPENSE_MANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Expense/Expense.hpp"
class ExpenseManager
{
private:
    DynamicArray<Expense> list;

public:
    ExpenseManager();
    void add(const Expense &w);
    void remove(int id);

    double getTotalBalance();
    DynamicArray<Expense> &getAll();
    void updateDB();
    double getBalanceInTimeRange(std::string start, std::string end);
    double anualExpenseOverview(DynamicArray<int> year);
    double expenseBreakdownByCategory(DynamicArray<int> year, const std::string &categoryName);
};

#endif