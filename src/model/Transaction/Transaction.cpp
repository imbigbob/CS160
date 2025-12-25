#include "Transaction.hpp"

#include <ctime>

std::string Transaction::getDate() const { return date; }
std::string Transaction::formatId()
{
    // Simple example: use current timestamp as ID
    std::time_t now = std::time(nullptr);
    return std::to_string(now);
}

double Transaction::getAmount() const { return amount; }

std::string Transaction::getWalletId() const { return walletId; }

std::string Transaction::getDescription() const { return description; }

std::string Transaction::getDate(const std::string &date) { return this->date; }

void Transaction::setAmount(double amount) { this->amount = amount; }

void Transaction::setWalletId(const std::string &walletId)
{
    this->walletId = walletId;
}

void Transaction::setDescription(const std::string &description)
{
    this->description = description;
}
