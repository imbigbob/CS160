#pragma once
#include <string>

#include "../Transaction/Transaction.hpp"
struct IncomeSource {
    int id;
    std::string name;
};

class Income : public Transaction {
   private:
    int sourceId;

   public:
    Income() : Transaction(), sourceId(0) {}

    Income(
        int id, double amount, std::string date, int walletId,
        std::string description, int source
    )
        : Transaction(id, amount, date, walletId, description),
          sourceId(source) {}

    int getType() const override { return 2; }
    int getSourceId() const;
    double getAmount() const;
    std::string getDate() const;
    int getWalletId() const;
    std::string getDescription() const;
    void setSourceId(int sourceId);
};
