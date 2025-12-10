
#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
class Transaction {
   protected:
    std::string date;  // or custom Date struct
    std::string id;
    std::string name;
    double amount;
    std::string walletId;
    std::string walletName;
    std::string description;

   private:
   public:
    Transaction()
        : id(""), amount(0.0), date(""), walletId(""), description("") {}

    Transaction(
        std::string date, std::string id, std::string name, double amount,
        std::string walletId, std::string walletName, std::string description
    ) {
        this->date = date;
        this->id = id;
        this->name = name;
        this->amount = amount;
        this->walletId = walletId;
        this->walletName = walletName;
        this->description = description;
    }

    virtual int getType() const = 0;  // 1=Income, 2=Expense
    std::string getDate() const;
    std::string setDate(std::string date) { return this->date = date; }
    double getAmount() const;
    void setAmount(double amount);

    std::string getId() const { return id; }
    void setId(const std::string& id) { this->id = id; }

    std::string getName() const { return name; }
    void setName(const std::string& name) { this->name = name; }

    std::string getDate(const std::string& date);

    std::string getWalletId() const;
    void setWalletId(const std::string& walletId);
    void setWalletName(const std::string& walletName) {
        this->walletName = walletName;
    }

    std::string getDescription() const;
    void setDescription(const std::string& description);
};

#endif
