#include <string>

#include "DynamicArray.hpp"
#include "TransactionManager/TransactionManager.hpp"
struct RecurringTransaction {
    int type;  // 1=Income, 2=Expense
    double amount;
    int walletId;
    int sourceIdOrCateId;
    std::string startDate;
    std::string endDate;  // "" means infinite
};

class RecurringManager {
   private:
    DynamicArray<RecurringTransaction> rules;

   public:
    void addRule(const RecurringTransaction&);
    void applyForCurrentMonth(TransactionManager& tm);
};
