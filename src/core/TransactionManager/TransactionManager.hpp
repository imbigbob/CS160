
#pragma once
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Expense/Expense.hpp"
#include "model/Income/Income.hpp"
#include "model/Wallet/Wallet.hpp"
class TransactionManager {
   private:
    DynamicArray<Income> incomes;
    DynamicArray<Expense> expenses;
    int nextId;

   public:
    TransactionManager();
    void addIncome(const Income& inc);
    void addExpense(const Expense& exp);

    DynamicArray<Income>& getIncomes();
    DynamicArray<Expense>& getExpenses();
};
