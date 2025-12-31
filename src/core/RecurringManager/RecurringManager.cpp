#include "RecurringManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <cstring>
#include "model/Transaction/Transaction.hpp"

// File path
RecurringManager::RecurringManager()
{
    this->incomeFilepath = "data/IncomeRecurring.json";
    this->expenseFilepath = "data/ExpenseRecurring.json";

    loadFromDB(incomeFilepath, incomeRules, 1);
    loadFromDB(expenseFilepath, expenseRules, 2);
}

// Rule adding
void RecurringManager::addRule(const RecurringTransaction &rule)
{

    // Adding rules
    switch (rule.typeTransaction)
    {
    case 1:
    {
        incomeRules.pushBack(rule);
        updateDB(incomeFilepath, incomeRules);

        return;
    }
    case 2:
    {
        expenseRules.pushBack(rule);
        updateDB(expenseFilepath, expenseRules);

        return;
    }
    }
}

// Current year month with format YYYY-MM
std::string RecurringManager::getCurrentYM()
{

    // ctime
    time_t t = time(nullptr);
    tm *now = localtime(&t);

    // stringstream lib
    std::stringstream ss;
    ss << (now->tm_year + 1900) << "-"
       << std::setw(2) << std::setfill('0') << (now->tm_mon + 1); // iomanip
    return ss.str();
}

// Read from json file
void RecurringManager::loadFromDB(const std::string &filepath,
                                  DynamicArray<RecurringTransaction> &list,
                                  int typeTransaction)
{
    std::ifstream file(filepath);
    if (!file.is_open())
        return;

    nlohmann::json root;
    try
    {
        file >> root;
    }
    catch (...)
    {
        return;
    }

    if (!root.is_array())
        return;

    for (const auto &obj : root)
    {
        RecurringTransaction r;
        r.type = typeTransaction;

        if (obj.contains("amount") && obj["amount"].is_number())
            r.amount = obj["amount"].get<double>();

        if (obj.contains("categoryId") && obj["categoryId"].is_string())
            r.categoryId = obj["categoryId"].get<std::string>();

        if (obj.contains("type") && obj["type"].is_string())
            r.type = obj["type"].get<std::string>();

        if (obj.contains("walletId") && obj["walletId"].is_string())
            r.walletId = obj["walletId"].get<std::string>();

        if (obj.contains("description") && obj["description"].is_string())
            r.description = obj["description"].get<std::string>();

        if (obj.contains("day") && obj["day"].is_number_integer())
            r.day = obj["day"].get<int>();

        if (obj.contains("startDate") && obj["startDate"].is_string())
            r.startDate = obj["startDate"].get<std::string>();

        if (obj.contains("endDate") && obj["endDate"].is_string())
            r.endDate = obj["endDate"].get<std::string>();

        if (obj.contains("lastAppliedYM") && obj["lastAppliedYM"].is_string())
            r.lastAppliedYM = obj["lastAppliedYM"].get<std::string>();

        list.pushBack(r);
    }
}

// Write to json file
void RecurringManager::updateDB(const std::string &filepath,
                                DynamicArray<RecurringTransaction> &list)
{
    nlohmann::json root = nlohmann::json::array();

    for (int i = 0; i < list.getSize(); i++)
    {
        RecurringTransaction &r = list[i];

        nlohmann::json obj;
        obj["amount"] = r.amount;
        obj["categoryId"] = r.categoryId;
        obj["type"] = r.type;
        obj["walletId"] = r.walletId;
        obj["description"] = r.description;
        obj["day"] = r.day;
        obj["startDate"] = r.startDate;
        obj["endDate"] = r.endDate;
        obj["lastAppliedYM"] = r.lastAppliedYM;
        root.push_back(obj);
    }

    std::ofstream file(filepath);
    if (!file.is_open())
        return;

    try
    {
        file << root.dump(4);
    }
    catch (...)
    {
        return;
    }
}

// Main
void RecurringManager::processRecurring()
{

    // Get current YYYY-MM
    std::string currentMonth = getCurrentYM();
    // Get current DD
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    int currentDay = now->tm_mday;

    // Get the current YYYY-MM-DD
    std::stringstream dateSS;
    dateSS << (now->tm_year + 1900) << "-"
           << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-"
           << std::setw(2) << std::setfill('0') << currentDay;
    std::string todayDate = dateSS.str();
    bool incomeProcessed = false;
    bool expenseProcessed = false;
    // Income recurring
    for (int i = 0; i < incomeRules.getSize(); ++i)
    {
        RecurringTransaction &rule = incomeRules[i];
        if (rule.lastAppliedYM == currentMonth)
            continue;
        if (currentDay < rule.day)
            continue;
        if (!rule.startDate.empty() && todayDate < rule.startDate)
            continue;
        if (!rule.endDate.empty() && todayDate > rule.endDate)
            continue;

        // Income profile
        Transaction inc(rule.type, rule.amount,
                        rule.walletId, rule.description + "[Auto Rec]");
        incomeManagers.add(inc); // from IncomeManager
        rule.lastAppliedYM = currentMonth;
        incomeProcessed = true;
        std::cout << ">> [Auto Income] Created: " << rule.description << "\n";
    }

    // Expense recurring
    for (int i = 0; i < expenseRules.getSize(); ++i)
    {
        RecurringTransaction &rule = expenseRules[i];
        if (rule.lastAppliedYM == currentMonth)
            continue;
        if (currentDay < rule.day)
            continue;
        if (!rule.startDate.empty() && todayDate < rule.startDate)
            continue;
        if (!rule.endDate.empty() && todayDate > rule.endDate)
            continue;

        // Expense profile
        Transaction exp(rule.type, rule.amount,
                        rule.walletId, rule.description + "[Auto Rec]");

        expenseManagers.add(exp); // from ExpenseManager
        expenseProcessed = true;
        rule.lastAppliedYM = currentMonth;
        std::cout << ">> [Auto Expense] Created: " << rule.description << "\n";
    }
    if (incomeProcessed)
        updateDB(incomeFilepath, incomeRules);
    if (expenseProcessed)
        updateDB(expenseFilepath, expenseRules);
    return;
}
void RecurringManager::deleteIncomeRule(int index)
{
    if (index < 0 || index >= incomeRules.getSize())
        return;
    incomeRules.removeAt(index);
    updateDB(incomeFilepath, incomeRules);
}
void RecurringManager::deleteExpenseRule(int index)
{
    if (index < 0 || index >= expenseRules.getSize())
        return;
    expenseRules.removeAt(index);
    updateDB(expenseFilepath, expenseRules);
}

void RecurringManager::editIncomeRule()
{
    // Implementation depends on how you want to edit the rule
    // This is a placeholder for the actual editing logic
    updateDB(incomeFilepath, incomeRules);
}

void RecurringManager::editExpenseRule()
{
    // Implementation depends on how you want to edit the rule
    // This is a placeholder for the actual editing logic
    updateDB(expenseFilepath, expenseRules);
}