

#include "TransactionManager.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <fstream>
#include <iostream>
TransactionManager::TransactionManager(std::string filePath)
{
    this->filePath = filePath;
    std::ifstream file(filePath);
    if (!file)
    {
        return;
    }
    if (!file.is_open())
    {
        list = DynamicArray<Transaction>();
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
        Transaction transaction;
        if (obj.contains("date"))
            transaction.setDate(obj["date"].get<std::string>());
        if (obj.contains("id"))
            transaction.setId(obj["id"].get<std::string>());
        if (obj.contains("type"))
            transaction.setTypeId(obj["type"].get<std::string>());
        if (obj.contains("amount"))
            transaction.setAmount(obj["amount"].get<double>());

        if (obj.contains("walletId"))
            transaction.setWalletId(obj["walletId"].get<std::string>());
        if (obj.contains("description"))
            transaction.setDescription(obj["description"].get<std::string>());
        list.pushBack(transaction);
    }
}

void TransactionManager::add(const Transaction &w)
{
    list.pushBack(w);
    updateDB();
    return;
}
void TransactionManager::remove(int id)
{
    if (id < 0 || id >= list.getSize())
    {
        std::cerr << "Invalid index for removal: " << id << std::endl;
        return;
    }
    list.removeAt(id);
    updateDB();
}

double TransactionManager::getTotalBalance()
{
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++)
    {
        total += list[i].getAmount();
    }
    return total;
}

DynamicArray<Transaction> &TransactionManager::getAll() { return list; }

void TransactionManager::updateDB()
{
    json root = json::array();

    // Convert DynamicArray<Income> → Json::Value array
    for (int i = 0; i < list.getSize(); i++)
    {
        const Transaction &transaction = list[i];
        json obj;
        obj["date"] = transaction.getDate();
        obj["id"] = transaction.getId();
        obj["type"] = transaction.getTypeId();
        obj["amount"] = transaction.getAmount();

        obj["walletId"] = transaction.getWalletId();
        obj["description"] = transaction.getDescription();

        root.push_back(obj);
    }
    // Write to file
    std::ofstream file(filePath);
    if (!file)
    {
        throw std::runtime_error("Error opening " + filePath + " for writing");
    }
    file << root.dump(2);
}

double TransactionManager::getBalanceInTimeRange(std::string start, std::string end)
{
    double totalBalance = 0.0;

    // Loop through the DynamicArray
    for (int i = 0; i < list.getSize(); ++i)
    {
        // 1. Get the current income object
        const Transaction &currentTransaction = list[i];

        // 2. Get the date string (e.g., "2024-06-01")
        std::string transactionDate = currentTransaction.getDate();

        // 3. String comparison works for YYYY-MM-DD format
        // Check if date is >= start AND <= end
        if (transactionDate >= start && transactionDate <= end)
        {
            totalBalance += currentTransaction.getAmount();
        }
    }

    // Return as int (or change function signature to return double)
    return totalBalance;
}

double TransactionManager::anualTransactionOverview(DynamicArray<int> year)
{
    double totalTransaction = 0.0;

    // 1. Loop through the list of years you want to check (e.g., 2023, 2024)
    for (int i = 0; i < year.getSize(); ++i)
    {
        int targetYear = year[i];
        std::string targetYearStr = std::to_string(targetYear);

        // 2. Loop through all income entries to find matches
        for (int j = 0; j < list.getSize(); ++j)
        {
            std::string incomeDate = list[j].getDate(); // Returns "YYYY-MM-DD"

            // 3. Check if the date has enough characters and matches the year
            // substr(0, 4) extracts the first 4 chars (The Year)
            if (incomeDate.length() >= 4 && incomeDate.substr(0, 4) == targetYearStr)
            {
                totalTransaction += list[j].getAmount();
            }
        }
    }

    return totalTransaction;
}

double TransactionManager::transactionBreakdownByType(DynamicArray<int> year, const std::string &type)
{
    double totalTransaction = 0.0;

    // 1. Loop through the list of years you want to check (e.g., 2023, 2024)
    for (int i = 0; i < year.getSize(); ++i)
    {
        int targetYear = year[i];
        std::string targetYearStr = std::to_string(targetYear);

        // 2. Loop through all transaction entries to find matches
        for (int j = 0; j < list.getSize(); ++j)
        {
            std::string transactionDate = list[j].getDate(); // Returns "YYYY-MM-DD"

            // 3. Check if the date has enough characters and matches the year
            // substr(0, 4) extracts the first 4 chars (The Year)
            if (transactionDate.length() >= 4 && transactionDate.substr(0, 4) == targetYearStr &&
                list[j].getTypeId() == type)
            {
                totalTransaction += list[j].getAmount();
            }
        }
    }

    return totalTransaction;
}

Transaction *TransactionManager::getValue(int index)
{
    if (index < 0 || index >= list.getSize())
    {
        return nullptr;
    }
    return &list[index];
}

void TransactionManager::editTransaction(int index, const Transaction &t)
{
    if (index < 0 || index >= list.getSize())
    {
        std::cerr << "Invalid index for editing: " << index << std::endl;
        return;
    }
    list[index] = t;
    updateDB();
}