#include "Transaction.hpp"
class Expense : public Transaction {
    int categoryId;

   public:
    int getType() const override { return 2; }
};
