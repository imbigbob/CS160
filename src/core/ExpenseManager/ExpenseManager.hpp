#ifndef EXPENSE_MANAGER_HPP
#define EXPENSE_MANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Expense/Expense.hpp"
class ExpenseManager {
   private:
    DynamicArray<Expense> list;

   public:
    ExpenseManager();
    void add(const Expense& w);
    void remove(int id);

    double getTotalBalance();
    DynamicArray<Expense>& getAll();
    void updateDB();
};

#endif