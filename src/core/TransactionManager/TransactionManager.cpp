#include "TransactionManager.hpp"
TransactionManager::TransactionManager() : nextId(1), incomes(), expenses() {}

void TransactionManager::addIncome(const Income& inc) {}

void TransactionManager::addExpense(const Expense& exp) {}

DynamicArray<Income>& TransactionManager::getIncomes() { return incomes; }

DynamicArray<Expense>& TransactionManager::getExpenses() { return expenses; }