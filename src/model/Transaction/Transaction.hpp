#include <string>
class Transaction {
   protected:
    int id;
    std::string date;  // or custom Date struct
    double amount;
    int walletId;
    std::string description;

   public:
    virtual int getType() const = 0;  // 1=Income, 2=Expense
    // Common getters/setters...
};
