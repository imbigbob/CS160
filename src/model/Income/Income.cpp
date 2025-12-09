#include "Income.hpp"

int Income::getSourceId() const { return sourceId; }
double Income::getAmount() const { return Transaction::getAmount(); }

std::string Income::getDate() const { return Transaction::getDate(); }

int Income::getWalletId() const { return Transaction::getWalletId(); }

std::string Income::getDescription() const {
    return Transaction::getDescription();
}

void Income::setSourceId(int sourceId) { this->sourceId = sourceId; }
