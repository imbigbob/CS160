#include "Transaction.hpp"

#include <ctime>

std::string Transaction::getDate() const { return date; }

double Transaction::getAmount() const { return amount; }

std::string Transaction::getWalletId() const { return walletId; }

std::string Transaction::getDescription() const { return description; }

std::string Transaction::getDate(const std::string& date) { return this->date; }

void Transaction::setAmount(double amount) { this->amount = amount; }

void Transaction::setWalletId(const std::string& walletId) {
    this->walletId = walletId;
}

void Transaction::setDescription(const std::string& description) {
    this->description = description;
}
