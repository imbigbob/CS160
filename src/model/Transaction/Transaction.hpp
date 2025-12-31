
#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
class Transaction
{
protected:
    std::string date; // or custom Date struct
    std::string id;
    std::string typeId;
    double amount;
    std::string walletId;
    std::string description;

private:
    std::string formatId();
    std::string initialDate();

public:
    Transaction()
        : id(""), amount(0.0), date(""), walletId(""), description("") {}

    Transaction(
        std::string typeId, double amount,
        std::string walletId, std::string description)
    {
        this->date = initialDate();
        this->id = formatId();
        this->typeId = typeId;
        this->amount = amount;
        this->walletId = walletId;
        this->description = description;
    }

    Transaction(
        std::string date, std::string id, std::string typeId, double amount,
        std::string walletId, std::string description)
    {
        this->date = date;
        this->id = id;
        this->typeId = typeId;
        this->amount = amount;
        this->walletId = walletId;
        this->description = description;
    }

    std::string getDate() const;
    std::string setDate(std::string date) { return this->date = date; }
    double getAmount() const;
    void setAmount(double amount);

    std::string getId() const { return id; }
    void setId(const std::string &id) { this->id = id; }
    std::string getTypeId() const { return typeId; }
    void setTypeId(const std::string &typeId) { this->typeId = typeId; }

    std::string getDate(const std::string &date);

    std::string getWalletId() const;
    void setWalletId(const std::string &walletId);

    std::string getDescription() const;
    void setDescription(const std::string &description);
};

#endif
