#ifndef EXPENSE_MANAGER_HPP
#define EXPENSE_MANAGER_HPP

#include <string>
#include "DynamicArray.hpp" 

class ExpenseCategory {
   private:
    std::string id;
    std::string name;

   public:
    ExpenseCategory() {}
    
    void setId(std::string i) { id = i; }
    void setName(std::string n) { name = n; }
    
    std::string getId() const { return id; }
    std::string getName() const { return name; }
};

class Expense {
   private:
    std::string id;
    std::string date;
    std::string name; 
    double amount;
    
    std::string walletId;
    std::string walletName; 
    
    std::string description;

   public:
    Expense() : amount(0.0) {}

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

class ExpenseManager {
   private:
    DynamicArray<Expense> list;
    DynamicArray<ExpenseCategory> categoryList; 

   private:
    void updateDB();          // Updates expenses.json
    void updateCategoryDB();  // Updates expensetypes.json

   public:
    ExpenseManager();
    
    void add(const Expense& w);
    void remove(std::string id);
    Expense* findById(std::string id);
    double getTotalBalance();
    DynamicArray<Expense>& getAll();
    
    void addCategory(const ExpenseCategory& t);
    void modifyCategory(const ExpenseCategory& t);
    void removeCategory(std::string id);
    DynamicArray<ExpenseCategory>& getAllCategories();
};

#endif