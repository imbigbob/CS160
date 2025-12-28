#include "Expense.hpp"

Expense::Expense() : Transaction()
{
}

Expense::Expense(
    std::string name, double amount,
    std::string walletId, std::string walletName, std::string description)
    : Transaction(name, amount, walletId, walletName, description),
      mWalletName(walletName) // Initialize local copy
{
}

std::string Expense::getWalletName() const
{
    return mWalletName;
}

void Expense::setWalletName(const std::string &walletName)
{
    mWalletName = walletName;
    Transaction::setWalletName(walletName);
}

int Expense::getType() const
{
    return 1; // Type ID for Expense (Assuming 1, or keep 2 if that was intentional in your code)
}