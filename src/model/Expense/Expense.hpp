// Expense.hpp
#ifndef EXPENSE_HPP
#define EXPENSE_HPP
#include "model/Transaction/Transaction.hpp"

class Expense : public Transaction
{
private:
public:
    Expense() : Transaction() {}

    Expense(
        std::string name, double amount,
        std::string walletId, std::string walletName, std::string description)
        : Transaction(
              name, amount, walletId, walletName, description) {}

    int getType() const override { return 2; }
};

#endif