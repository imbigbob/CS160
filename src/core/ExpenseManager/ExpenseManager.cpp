#include "ExpenseManager.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ExpenseManager::ExpenseManager()
{
    std::ifstream file("data/ExpensesTransaction.json");
    
    if (!file || !file.is_open())
    {
        list = DynamicArray<Expense>();
        return;
    }

    // Check if file is empty
    if (file.peek() == std::ifstream::traits_type::eof()) {
        list = DynamicArray<Expense>();
        return;
    }

    json root;
    try {
        file >> root;
    } catch (...) {
        return;
    }

    if (!root.is_array())
    {
        return; // Invalid format
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
}

void ExpenseManager::remove(int index) {
    if (index >= 0 && index < list.getSize()) {
        list.removeAt(index);
        updateDB();
    }
}

double ExpenseManager::getTotalBalance()
{
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++)
    {
        total += list[i].getAmount();
    }
    return total;
}

DynamicArray<Expense> &ExpenseManager::getAll() { 
    return list; 
}

void ExpenseManager::updateDB()
{
    json root = json::array();

    for (int i = 0; i < list.getSize(); i++)
    {
        const Expense &expense = list[i];
        json obj;
        obj["date"] = expense.getDate();
        obj["id"] = expense.getId();
        obj["categoryName"] = expense.getName();
        obj["amount"] = expense.getAmount();

        obj["walletId"] = expense.getWalletId();

        // [FIX IS HERE] Save the actual wallet name instead of ""
        obj["walletName"] = expense.getWalletName(); 
        
        obj["description"] = expense.getDescription();

        root.push_back(obj);
    }

    std::ofstream file("data/ExpensesTransaction.json");
    if (!file)
    {
        std::cerr << "Error opening data/ExpensesTransaction.json for writing" << std::endl;
        return;
    }
    file << root.dump(4);
}

double ExpenseManager::getBalanceInTimeRange(std::string start, std::string end)
{
    double totalBalance = 0.0;
    for (int i = 0; i < list.getSize(); ++i)
    {
        const Expense &currentExpense = list[i];
        std::string expenseDate = currentExpense.getDate();

        if (expenseDate >= start && expenseDate <= end)
        {
            totalBalance += currentExpense.getAmount();
        }
    }
    return totalBalance;
}

double ExpenseManager::anualExpenseOverview(DynamicArray<int> year)
{
    double totalExpense = 0.0;
    for (int i = 0; i < year.getSize(); ++i)
    {
        int targetYear = year[i];
        std::string targetYearStr = std::to_string(targetYear);

        for (int j = 0; j < list.getSize(); ++j)
        {
            std::string expenseDate = list[j].getDate(); 
            if (expenseDate.length() >= 4 && expenseDate.substr(0, 4) == targetYearStr)
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
    for (int i = 0; i < year.getSize(); ++i)
    {
        int targetYear = year[i];
        std::string targetYearStr = std::to_string(targetYear);

        for (int j = 0; j < list.getSize(); ++j)
        {
            std::string expenseDate = list[j].getDate(); 
            if (expenseDate.length() >= 4 && expenseDate.substr(0, 4) == targetYearStr &&
                list[j].getName() == categoryName)
            {
                totalExpense += list[j].getAmount();
            }
        }
    }
    return totalExpense;
}