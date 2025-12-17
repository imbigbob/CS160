#ifndef WALLET_MANAGER_HPP
#define WALLET_MANAGER_HPP

#include <string>
#include "DynamicArray.hpp"

class Wallet {
   private:
    std::string id;
    std::string name;
    double balance;

   public:
    Wallet() : balance(0.0) {}

    void setId(std::string i) { id = i; }
    void setName(std::string n) { name = n; }
    void setBalance(double b) { balance = b; }

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    double getBalance() const { return balance; }
};


class WalletManager {
   private:
    DynamicArray<Wallet> list;
    
    void updateDB(); // Updates wallets.json

   public:
    WalletManager();
    
    void add(const Wallet& w);
    void remove(std::string id);    
    Wallet* findById(std::string id); 
    
    double getTotalBalance(); 
    DynamicArray<Wallet>& getAll();
};

#endif