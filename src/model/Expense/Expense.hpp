#ifndef EXPENSE_HPP
#define EXPENSE_HPP

#include <string>
#include "../Transaction/Transaction.hpp"

class Expense : public Transaction
{
public:
    Expense();

    Expense(
        std::string name, double amount,
        std::string walletId, std::string walletName, std::string description);

    // [FIX] Add the missing getter
    std::string getWalletName() const;

    // [FIX] Override setter
    void setWalletName(const std::string& walletName);

    int getType() const override;

private:
    // [FIX] Local storage
    std::string mWalletName;
};

#endif