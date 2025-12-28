#ifndef INCOME_HPP
#define INCOME_HPP

#include <string>
#include "../Transaction/Transaction.hpp"

class Income : public Transaction
{
public:
    Income();

    Income(
        std::string name, double amount,
        std::string walletId, std::string walletName, std::string description);

    // [FIX] Add the missing getter explicitly
    std::string getWalletName() const;
    
    // [FIX] Override setter to keep our local copy in sync
    void setWalletName(const std::string& walletName);

    int getType() const override;

private:
    // [FIX] Local storage to ensure we can get the value
    std::string mWalletName;
};

#endif