#ifndef WALLET_HPP
#define WALLET_HPP
#include <string>
#include <ctime>
class Wallet
{
public:
    std::string id;
    std::string name;
    double balance; // computed or stored

private:
    std::string formatId()
    {
        // Simple example: use current timestamp as ID
        std::time_t now = std::time(nullptr);
        return std::to_string(now);
    }

public:
    Wallet() : id(""), name(""), balance(0.0) {}
    Wallet(const std::string &name, double balance)
        : name(name), balance(balance)
    {
        id = formatId();
    }

    Wallet(std::string id, const std::string &name, double balance)
        : id(id), name(name), balance(balance) {}
    std::string getId() const { return id; }

    std::string getName() const { return name; }
    void setName(const std::string &newName) { name = newName; }

    double getBalance() const { return balance; }
    void setBalance(double newBalance) { balance = newBalance; }
    void adjustBalance(double amount) { balance += amount; }
};
#endif