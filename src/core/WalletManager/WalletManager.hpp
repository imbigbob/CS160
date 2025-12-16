#ifndef WALLET_MANAGER_HPP
#define WALLET_MANAGER_HPP

#include <string>
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Wallet/Wallet.hpp" 

// --- Helper Structures (Defined internally as requested) ---
struct IncomeType {
    std::string id;
    std::string name;
    IncomeType() {}
    IncomeType(std::string id, std::string name) : id(id), name(name) {}
};

struct ExpenseType {
    std::string id;
    std::string name;
    ExpenseType() {}
    ExpenseType(std::string id, std::string name) : id(id), name(name) {}
};

class WalletManager {
   private:
    // --- Data Lists ---
    DynamicArray<Wallet> wallets;
    DynamicArray<IncomeType> incomeTypes;
    DynamicArray<ExpenseType> expenseTypes;
    
    // --- File Paths ---
    const std::string WALLET_FILE = "data/WalletTypes.json";
    const std::string INCOME_TYPE_FILE = "data/IncomeTypes.json";
    const std::string EXPENSE_TYPE_FILE = "data/ExpenseTypes.json";
    
    // Transaction files (Read-Only for balance calc)
    const std::string INCOME_TX_FILE = "data/IncomesTransaction.json";
    const std::string EXPENSE_TX_FILE = "data/ExpensesTransaction.json";

    // --- Internal Helpers ---
    void loadAll();
    void calculateBalances();

   public:
    WalletManager();
    
    // Re-reads files and recalculates balance
    void reloadData(); 
    
    // Call this after editing a Name to persist changes
    void save(); 

    // --- WALLET OPERATIONS ---
    void addWallet(const Wallet& w);
    void removeWallet(std::string id);
    Wallet* findWalletById(std::string id);
    DynamicArray<Wallet>& getWallets();

    // --- INCOME SOURCE OPERATIONS ---
    void addIncomeType(const IncomeType& t);
    void removeIncomeType(std::string id);
    IncomeType* findIncomeTypeById(std::string id);
    DynamicArray<IncomeType>& getIncomeTypes();

    // --- EXPENSE CATEGORY OPERATIONS ---
    void addExpenseType(const ExpenseType& t);
    void removeExpenseType(std::string id);
    ExpenseType* findExpenseTypeById(std::string id);
    DynamicArray<ExpenseType>& getExpenseTypes();
};

#endif