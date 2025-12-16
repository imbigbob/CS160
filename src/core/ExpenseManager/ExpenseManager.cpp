#include "ExpenseManager.hpp"
#include <iostream>
#include <fstream>
#include <json/json.h>

// Standardized file path
const std::string FILE_PATH = "data/ExpensesTransaction.json";

ExpenseManager::ExpenseManager() {
    std::ifstream file(FILE_PATH);
    if (!file) {
        // File doesn't exist yet, start with empty list
        return;
    }

    Json::Value root;
    Json::CharReaderBuilder reader;
    std::string errs;

    if (!Json::parseFromStream(reader, file, &root, &errs)) {
        std::cerr << "Error parsing " << FILE_PATH << ": " << errs << std::endl;
        return;
    }

    if (!root.isArray()) {
        std::cerr << "Invalid JSON format: root is not an array" << std::endl;
        return;
    }

    for (const auto& obj : root) {
        Expense expense;
        if (obj.isMember("date")) expense.setDate(obj["date"].asString());
        if (obj.isMember("id")) expense.setId(obj["id"].asString());
        
        // "categoryName" in JSON maps to Transaction::name
        if (obj.isMember("categoryName")) expense.setName(obj["categoryName"].asString());
        
        if (obj.isMember("amount")) expense.setAmount(obj["amount"].asDouble());
        if (obj.isMember("walletId")) expense.setWalletId(obj["walletId"].asString());
        if (obj.isMember("walletName")) expense.setWalletName(obj["walletName"].asString());
        if (obj.isMember("description")) expense.setDescription(obj["description"].asString());

        list.pushBack(expense);
    }
}

void ExpenseManager::add(const Expense& w) {
    list.pushBack(w);
    updateDB();
}

void ExpenseManager::remove(std::string id) {
    for (int i = 0; i < list.getSize(); i++) {
        if (list[i].getId() == id) {
            list.removeAt(i);
            updateDB();
            return;
        }
    }
    std::cout << "ID not found: " << id << std::endl;
}

Expense* ExpenseManager::findById(std::string id) {
    for (int i = 0; i < list.getSize(); i++) {
        if (list[i].getId() == id) {
            return &list[i];
        }
    }
    return nullptr;
}

double ExpenseManager::getTotalBalance() {
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++) {
        total += list[i].getAmount();
    }
    return total;
}

DynamicArray<Expense>& ExpenseManager::getAll() {
    return list;
}

void ExpenseManager::updateDB() {
    Json::Value root(Json::arrayValue);

    for (int i = 0; i < list.getSize(); i++) {
        const Expense& expense = list[i];
        Json::Value obj;
        
        obj["date"] = expense.getDate();
        obj["id"] = expense.getId();
        
        // Save Name as "categoryName" to match requirements
        obj["categoryName"] = expense.getName();
        
        obj["amount"] = expense.getAmount();
        obj["walletId"] = expense.getWalletId();
        
        // Correctly saving the wallet name now
        obj["walletName"] = expense.getWalletName(); 
        
        obj["description"] = expense.getDescription();

        root.append(obj);
    }

    std::ofstream file(FILE_PATH);
    if (!file) {
        std::cerr << "Error opening " << FILE_PATH << " for writing" << std::endl;
        return;
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  "; // Pretty print
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}