#pragma once
#include <string>

#include "Transaction.hpp"
class Income : public Transaction {
    int sourceId;

   public:
    int getType() const override { return 1; }
};
