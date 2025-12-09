#include "Transaction.hpp"

Transaction::Transaction(
    int id, double amount, std::string date, int walletId,
    std::string description
)
    : id(id),
      amount(amount),
      date(date),
      walletId(walletId),
      description(description) {}

std::string Transaction::getDate() const { return date; }

double Transaction::getAmount() const { return amount; }

int Transaction::getWalletId() const { return walletId; }

std::string Transaction::getDescription() const { return description; }

void Transaction::setDate(const std::string& date) { this->date = date; }

std::string Transaction::getDate(const std::string& date) { return this->date; }

void Transaction::setAmount(double amount) { this->amount = amount; }

void Transaction::setWalletId(int walletId) { this->walletId = walletId; }

void Transaction::setDescription(const std::string& description) {
    this->description = description;
}
