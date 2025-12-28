#include "Income.hpp"

Income::Income() : Transaction()
{
}

Income::Income(
    std::string name, double amount,
    std::string walletId, std::string walletName, std::string description)
    : Transaction(name, amount, walletId, walletName, description),
      mWalletName(walletName) // Initialize local copy
{
}

std::string Income::getWalletName() const
{
    return mWalletName;
}

void Income::setWalletName(const std::string &walletName)
{
    // Update local copy
    mWalletName = walletName;
    
    // Update parent copy (so Transaction logic still works if needed)
    Transaction::setWalletName(walletName);
}

int Income::getType() const
{
    return 2; // Type ID for Income
}