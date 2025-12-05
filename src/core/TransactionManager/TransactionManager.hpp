
#pragma once
#include "DynamicArray.hpp"
#include "model/Expense/Expense.hpp"
#include "model/Income/Income.hpp"
#include "model/Wallet/Wallet.hpp"
class TransactionManager {
   private:
    DynamicArray<Income> incomes;
    DynamicArray<Expense> expenses;
    int nextId;

   public:
    void addIncome(const Income& inc);
    void addExpense(const Expense& exp);

    DynamicArray<Income>& getIncomes();
    DynamicArray<Expense>& getExpenses();
};
