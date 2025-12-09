
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
    Transaction()
        : id(0), amount(0.0), date(""), walletId(0), description("") {}
    Transaction(
        int id, double amount, std::string date, int walletId,
        std::string description
    );

    virtual int getType() const = 0;  // 1=Income, 2=Expense
    std::string getDate() const;
    double getAmount() const;
    int getWalletId() const;
    std::string getDescription() const;
    void setDate(const std::string& date);
    std::string getDate(const std::string& date);
    void setAmount(double amount);
    void setWalletId(int walletId);
    void setDescription(const std::string& description);
};

#endif
