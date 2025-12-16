#ifndef EXPENSE_HPP
#define EXPENSE_HPP
#include <string>
#include "../Transaction/Transaction.hpp"

class Expense : public Transaction {
   public:
    Expense() : Transaction() {}

    Expense(
        std::string date, std::string id, std::string name, double amount,
        std::string walletId, std::string walletName, std::string description
    )
        : Transaction(
              date, id, name, amount, walletId, walletName, description
          ) {}

    // 2 represents Expense
    int getType() const override { return 2; }

    // Expose protected walletName from Transaction base class
    std::string getWalletName() const { return walletName; }
};

#endif