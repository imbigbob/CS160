
#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
class Transaction {
   protected:
    int id;
    std::string date;  // or custom Date struct
    double amount;
    int walletId;
    std::string description;
    
   public:
   Transaction() : id(0), amount(0.0), date(""), walletId(0), description("") {}
   Transaction(int id, double amount, std::string date, int walletId,
               std::string description);
        
    virtual int getType() const = 0;  // 1=Income, 2=Expense
    std::string getDate() const;
    // Common getters/setters...
};

#endif
