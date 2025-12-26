

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

double ExpenseManager::getTotalBalance()
{
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++)
    {
        total += list[i].getAmount();
    }
    return total;
}

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
    std::ofstream file("data/ExpensesTransaction.json");
    if (!file)
    {
        throw std::runtime_error("Error opening data/ExpensesTransaction.json for writing");
    }
}

double ExpenseManager::getBalanceInTimeRange(std::string start, std::string end)
{
    double totalBalance = 0.0;

    // Loop through the DynamicArray
    for (int i = 0; i < list.getSize(); ++i)
    {
        // 1. Get the current income object
        const Expense &currentExpense = list[i];

        // 2. Get the date string (e.g., "2024-06-01")
        std::string expenseDate = currentExpense.getDate();

        // 3. String comparison works for YYYY-MM-DD format
        // Check if date is >= start AND <= end
        if (expenseDate >= start && expenseDate <= end)
        {
            totalBalance += currentExpense.getAmount();
        }
    }

    // Return as int (or change function signature to return double)
    return totalBalance;
}

double ExpenseManager::anualExpenseOverview(DynamicArray<int> year)
{
    double totalExpense = 0.0;

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
                totalExpense += list[j].getAmount();
            }
        }
    }

    return totalExpense;
}

double ExpenseManager::expenseBreakdownByCategory(DynamicArray<int> year, const std::string &categoryName)
{
    double totalExpense = 0.0;

    // 1. Loop through the list of years you want to check (e.g., 2023, 2024)
    for (int i = 0; i < year.getSize(); ++i)
    {
        int targetYear = year[i];
        std::string targetYearStr = std::to_string(targetYear);

        // 2. Loop through all expense entries to find matches
        for (int j = 0; j < list.getSize(); ++j)
        {
            std::string expenseDate = list[j].getDate(); // Returns "YYYY-MM-DD"

            // 3. Check if the date has enough characters and matches the year
            // substr(0, 4) extracts the first 4 chars (The Year)
            if (expenseDate.length() >= 4 && expenseDate.substr(0, 4) == targetYearStr &&
                list[j].getName() == categoryName)
            {
                totalExpense += list[j].getAmount();
            }
        }
    }

    return totalExpense;
}