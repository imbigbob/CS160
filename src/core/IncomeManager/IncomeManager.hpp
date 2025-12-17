#ifndef INCOME_MANAGER_HPP
#define INCOME_MANAGER_HPP

#include <string>
#include "DynamicArray.hpp"


class IncomeType {
   private:
    std::string id;
    std::string name;

   public:
    IncomeType() {}
    
    void setId(std::string i) { id = i; }
    void setName(std::string n) { name = n; }
    
    std::string getId() const { return id; }
    std::string getName() const { return name; }
};


class Income {
   private:
    std::string id;
    std::string date;
    std::string name; 
    double amount;
    
    std::string walletId;
    std::string walletName; 
    
    std::string description;

   public:
    Income() : amount(0.0) {}

    void setId(std::string val) { id = val; }
    void setDate(std::string val) { date = val; }
    void setName(std::string val) { name = val; }
    void setAmount(double val) { amount = val; }
    void setWalletId(std::string val) { walletId = val; }
    void setWalletName(std::string val) { walletName = val; }
    void setDescription(std::string val) { description = val; }

   
    std::string getId() const { return id; }
    std::string getDate() const { return date; }
    std::string getName() const { return name; }
    double getAmount() const { return amount; }
    std::string getWalletId() const { return walletId; }
    std::string getWalletName() const { return walletName; }
    std::string getDescription() const { return description; }
};

class IncomeManager {
   private:
    DynamicArray<Income> list;
    DynamicArray<IncomeType> typeList; 

   private:
    void updateDB();      // Updates incomes.json
    void updateTypeDB();  // Updates incometypes.json

   public:
    IncomeManager();
    
    void add(const Income& w);
    void remove(std::string id);
    Income* findById(std::string id);
    double getTotalBalance();
    DynamicArray<Income>& getAll();
    
    void addType(const IncomeType& t);
    void modifyType(const IncomeType& t);
    void removeType(std::string id);
    DynamicArray<IncomeType>& getAllTypes();
};

#endif