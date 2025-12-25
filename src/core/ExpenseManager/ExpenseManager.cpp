

#include "ExpenseManager.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <fstream>
#include <iostream>
ExpenseManager::ExpenseManager()
{
    std::ifstream file("data/ExpensesTransaction.json");
    if (!file)
    {
        return;
    }
    if (!file.is_open())
    {
        list = DynamicArray<Expense>();
        return;
    }

    json root;
    file >> root;
    std::string errs;

    if (!root.is_array())
    {
        throw std::runtime_error("Invalid JSON format: root is not an array");
    }

    for (const auto &obj : root)
    {
        Expense expense;
        if (obj.contains("date"))
            expense.setDate(obj["date"].get<std::string>());
        if (obj.contains("id"))
            expense.setId(obj["id"].get<std::string>());
        if (obj.contains("categoryName"))
            expense.setName(obj["categoryName"].get<std::string>());
        if (obj.contains("amount"))
            expense.setAmount(obj["amount"].get<double>());

        if (obj.contains("walletId"))
            expense.setWalletId(obj["walletId"].get<std::string>());
        if (obj.contains("walletName"))
            expense.setWalletName(obj["walletName"].get<std::string>());
        if (obj.contains("description"))
            expense.setDescription(obj["description"].get<std::string>());
        list.pushBack(expense);
    }
}

void ExpenseManager::add(const Expense &w)
{
    list.pushBack(w);
    updateDB();
    return;
}
void ExpenseManager::remove(int id) {}

double ExpenseManager::getTotalBalance() { return 0; }

DynamicArray<Expense> &ExpenseManager::getAll() { return list; }

void ExpenseManager::updateDB()
{
    json root = json::array();

    // Convert DynamicArray<Income> → Json::Value array
    for (int i = 0; i < list.getSize(); i++)
    {
        const Expense &expense = list[i];
        json obj;
        obj["date"] = expense.getDate();
        obj["id"] = expense.getId();
        obj["categoryName"] = expense.getName();
        obj["amount"] = expense.getAmount();

        obj["walletId"] = expense.getWalletId();
        obj["walletName"] = ""; // Placeholder, as Wallet name is not in Income
        obj["description"] = expense.getDescription();

        root.push_back(obj);
    }
    // Write to file
    std::ofstream file("data/expenses.json");
    if (!file)
    {
        throw std::runtime_error("Error opening expenses.json for writing");
    }
}
