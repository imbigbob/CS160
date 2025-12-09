#pragma once
#include <string>

#include "../Transaction/Transaction.hpp"

class Income : public Transaction {
   private:
   public:
    Income() : Transaction() {}

    Income(
        std::string date, std::string id, std::string name, double amount,
        std::string walletId, std::string walletName, std::string description
    )
        : Transaction(
              date, id, name, amount, walletId, walletName, description
          ) {}

    int getType() const override { return 2; }
};
