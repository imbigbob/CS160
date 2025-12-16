#include "WalletManager.hpp"
#include <iostream>
#include <fstream>
#include <json/json.h>

WalletManager::WalletManager() {
    reloadData();
}

void WalletManager::reloadData() {
    // Clear existing data
    wallets.clear();
    incomeTypes.clear();
    expenseTypes.clear();
    
    // Load definitions
    loadAll();
    
    // Calculate money based on transaction history
    calculateBalances();
}

// ======================= LOADING =======================
void WalletManager::loadAll() {
    Json::CharReaderBuilder reader;
    std::string errs;

    // 1. Load Wallets
    std::ifstream wFile(WALLET_FILE);
    if (wFile) {
        Json::Value root;
        if (Json::parseFromStream(reader, wFile, &root, &errs)) {
            for (const auto& obj : root) {
                wallets.pushBack(Wallet(obj["id"].asString(), obj["name"].asString()));
            }
        }
    }

    // 2. Load Income Types
    std::ifstream iFile(INCOME_TYPE_FILE);
    if (iFile) {
        Json::Value root;
        if (Json::parseFromStream(reader, iFile, &root, &errs)) {
            for (const auto& obj : root) {
                incomeTypes.pushBack(IncomeType(obj["id"].asString(), obj["name"].asString()));
            }
        }
    }

    // 3. Load Expense Types
    std::ifstream eFile(EXPENSE_TYPE_FILE);
    if (eFile) {
        Json::Value root;
        if (Json::parseFromStream(reader, eFile, &root, &errs)) {
            for (const auto& obj : root) {
                expenseTypes.pushBack(ExpenseType(obj["id"].asString(), obj["name"].asString()));
            }
        }
    }
}

// ======================= BALANCE CALCULATION =======================
void WalletManager::calculateBalances() {
    Json::CharReaderBuilder reader;
    std::string errs;

    // 1. Add Incomes
    std::ifstream inFile(INCOME_TX_FILE);
    if (inFile) {
        Json::Value root;
        if (Json::parseFromStream(reader, inFile, &root, &errs)) {
            for (const auto& txn : root) {
                std::string wId = txn["walletId"].asString();
                double amt = txn["amount"].asDouble();
                Wallet* w = findWalletById(wId);
                if (w) w->adjustBalance(amt);
            }
        }
    }

    // 2. Subtract Expenses
    std::ifstream exFile(EXPENSE_TX_FILE);
    if (exFile) {
        Json::Value root;
        if (Json::parseFromStream(reader, exFile, &root, &errs)) {
            for (const auto& txn : root) {
                std::string wId = txn["walletId"].asString();
                double amt = txn["amount"].asDouble();
                Wallet* w = findWalletById(wId);
                if (w) w->adjustBalance(-amt);
            }
        }
    }
}

// ======================= SAVING =======================
void WalletManager::save() {
    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());

    // Save Wallets
    Json::Value wRoot(Json::arrayValue);
    for (int i = 0; i < wallets.getSize(); i++) {
        Json::Value obj;
        obj["id"] = wallets[i].getId();
        obj["name"] = wallets[i].getName();
        // Balance is not saved; it's calculated
        wRoot.append(obj);
    }
    std::ofstream wFile(WALLET_FILE);
    jsonWriter->write(wRoot, &wFile);

    // Save Income Types
    Json::Value iRoot(Json::arrayValue);
    for (int i = 0; i < incomeTypes.getSize(); i++) {
        Json::Value obj;
        obj["id"] = incomeTypes[i].id;
        obj["name"] = incomeTypes[i].name;
        iRoot.append(obj);
    }
    std::ofstream iFile(INCOME_TYPE_FILE);
    jsonWriter->write(iRoot, &iFile);

    // Save Expense Types
    Json::Value eRoot(Json::arrayValue);
    for (int i = 0; i < expenseTypes.getSize(); i++) {
        Json::Value obj;
        obj["id"] = expenseTypes[i].id;
        obj["name"] = expenseTypes[i].name;
        eRoot.append(obj);
    }
    std::ofstream eFile(EXPENSE_TYPE_FILE);
    jsonWriter->write(eRoot, &eFile);
}

// ======================= CRUD OPERATIONS =======================

// --- WALLETS ---
void WalletManager::addWallet(const Wallet& w) {
    wallets.pushBack(w);
    save();
}
void WalletManager::removeWallet(std::string id) {
    for (int i = 0; i < wallets.getSize(); i++) {
        if (wallets[i].getId() == id) {
            wallets.removeAt(i);
            save();
            return;
        }
    }
}
Wallet* WalletManager::findWalletById(std::string id) {
    for (int i = 0; i < wallets.getSize(); i++) {
        if (wallets[i].getId() == id) return &wallets[i];
    }
    return nullptr;
}
DynamicArray<Wallet>& WalletManager::getWallets() { return wallets; }

// --- INCOME TYPES ---
void WalletManager::addIncomeType(const IncomeType& t) {
    incomeTypes.pushBack(t);
    save();
}
void WalletManager::removeIncomeType(std::string id) {
    for (int i = 0; i < incomeTypes.getSize(); i++) {
        if (incomeTypes[i].id == id) {
            incomeTypes.removeAt(i);
            save();
            return;
        }
    }
}
IncomeType* WalletManager::findIncomeTypeById(std::string id) {
    for (int i = 0; i < incomeTypes.getSize(); i++) {
        if (incomeTypes[i].id == id) return &incomeTypes[i];
    }
    return nullptr;
}
DynamicArray<IncomeType>& WalletManager::getIncomeTypes() { return incomeTypes; }

// --- EXPENSE TYPES ---
void WalletManager::addExpenseType(const ExpenseType& t) {
    expenseTypes.pushBack(t);
    save();
}
void WalletManager::removeExpenseType(std::string id) {
    for (int i = 0; i < expenseTypes.getSize(); i++) {
        if (expenseTypes[i].id == id) {
            expenseTypes.removeAt(i);
            save();
            return;
        }
    }
}
ExpenseType* WalletManager::findExpenseTypeById(std::string id) {
    for (int i = 0; i < expenseTypes.getSize(); i++) {
        if (expenseTypes[i].id == id) return &expenseTypes[i];
    }
    return nullptr;
}
DynamicArray<ExpenseType>& WalletManager::getExpenseTypes() { return expenseTypes; }