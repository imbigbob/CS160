// Expense.hpp
#ifndef EXPENSE_HPP
#define EXPENSE_HPP
#include "model/Transaction/Transaction.hpp"

class Expense : public Transaction {
    private:
    int categoryId;

public:
    Expense() : Transaction(), categoryId(0) {}
    
    Expense(int id, double amount, std::string date, int walletId,
            std::string description, int category)
        : Transaction(id, amount, date, walletId, description), categoryId(category) {}
    
    int getType() const override { return 2; }
    int getCategoryId() const;
};

#endif