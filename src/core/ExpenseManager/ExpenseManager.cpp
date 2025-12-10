

#include "ExpenseManager.hpp"

#include <json/json.h>

#include <fstream>
#include <iostream>
ExpenseManager::ExpenseManager() {
    std::ifstream file("data/ExpensesTransaction.json");
    if (!file) {
        return;
    }
    if (!file.is_open()) {
        list = DynamicArray<Expense>();
        return;
    }
    Json::Value root;
    Json::CharReaderBuilder reader;
    std::string errs;

    if (!Json::parseFromStream(reader, file, &root, &errs)) {
        throw std::runtime_error("Error parsing expenses.json: " + errs);
    }

    if (!root.isArray()) {
        throw std::runtime_error("Invalid JSON format: root is not an array");
    }

    for (const auto& obj : root) {
        Expense expense;
        if (obj.isMember("date")) expense.setDate(obj["date"].asString());
        if (obj.isMember("id")) expense.setId(obj["id"].asString());
        if (obj.isMember("categoryName"))
            expense.setName(obj["categoryName"].asString());
        if (obj.isMember("amount")) expense.setAmount(obj["amount"].asDouble());

        if (obj.isMember("walletId"))
            expense.setWalletId(obj["walletId"].asString());
        if (obj.isMember("walletName"))
            expense.setWalletName(obj["walletName"].asString());
        if (obj.isMember("description"))
            expense.setDescription(obj["description"].asString());

        list.pushBack(expense);
    }
}

void ExpenseManager::add(const Expense& w) {
    list.pushBack(w);
    updateDB();
    return;
}
void ExpenseManager::remove(int id) {}

double ExpenseManager::getTotalBalance() { return 0; }

DynamicArray<Expense>& ExpenseManager::getAll() { return list; }

void ExpenseManager::updateDB() {
    Json::Value root(Json::arrayValue);

    // Convert DynamicArray<Income> → Json::Value array
    for (int i = 0; i < list.getSize(); i++) {
        const Expense& expense = list[i];
        Json::Value obj;
        obj["date"] = expense.getDate();
        obj["id"] = expense.getId();
        obj["categoryName"] = expense.getName();
        obj["amount"] = expense.getAmount();

        obj["walletId"] = expense.getWalletId();
        obj["walletName"] = "";  // Placeholder, as Wallet name is not in Income
        obj["description"] = expense.getDescription();

        root.append(obj);
    }
    // Write to file
    std::ofstream file("data/expenses.json");
    if (!file) {
        throw std::runtime_error("Error opening expenses.json for writing");
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";  // pretty print
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}
