#ifndef WALLET_HPP
#define WALLET_HPP

#include <string>

class Wallet {
   private:
    std::string id;
    std::string name;
    double balance;

   public:
    Wallet() : id(""), name(""), balance(0.0) {}
    
    Wallet(std::string id, std::string name) 
        : id(id), name(name), balance(0.0) {}

    std::string getId() const { return id; }
    void setId(std::string id) { this->id = id; }

    std::string getName() const { return name; }
    void setName(std::string name) { this->name = name; }

    double getBalance() const { return balance; }
    void setBalance(double balance) { this->balance = balance; }
    
    // Helper to adjust balance (used by Manager)
    void adjustBalance(double amount) { this->balance += amount; }
};

#endif