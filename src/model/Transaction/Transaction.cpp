#include "Transaction.hpp"


Transaction::Transaction(int id, double amount, std::string date, int walletId,
                       std::string description)
    : id(id), amount(amount), date(date), walletId(walletId),
      description(description) {}

std::string Transaction::getDate() const {
    return date;
}