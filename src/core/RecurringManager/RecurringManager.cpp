#include "RecurringManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <nlohmann/json.hpp>

#include "model/Income/Income.hpp"
#include "model/Expense/Expense.hpp"

// File path
RecurringManager::RecurringManager() {
    this->incomeFilepath = "data/IncomeRecurring.json";
    this->expenseFilepath = "data/ExpenseRecurring.json";

    loadFromDB(incomeFilepath, incomeRules, 1);
    loadFromDB(expenseFilepath, expenseRules, 2);
}

// Rule adding
void RecurringManager::addRule(const RecurringTransaction& rule) {

    // Adding rules
    switch(rule.type) {
        case 1: {
            incomeRules.pushBack(rule);
            updateDB(incomeFilepath, incomeRules);

            return;
        }
        case 2: {
            expenseRules.pushBack(rule);
            updateDB(expenseFilepath, expenseRules);         

            return;
        }
    }
}

// Current year month with format YYYY-MM
std::string RecurringManager::getCurrentYM() {

    // ctime
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    
    // stringstream lib
    std::stringstream ss;
    ss << (now->tm_year + 1900) << "-" 
       << std::setw(2) << std::setfill('0') << (now->tm_mon + 1); // iomanip
    return ss.str();
}

// Read from json file
void RecurringManager::loadFromDB(const std::string& filepath,
                                  DynamicArray<RecurringTransaction>& list,
                                  int typeTransaction) {
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    nlohmann::json root;
    try {
        file >> root;
    } catch (...) {
        return;
    }

    if (!root.is_array()) return;

    for (const auto& obj : root) {
        RecurringTransaction r;
        r.type = typeTransaction;

        if (obj.contains("amount") && obj["amount"].is_number())
            r.amount = obj["amount"].get<double>();

        if (obj.contains("categoryId") && obj["categoryId"].is_string())
            r.categoryId = obj["categoryId"].get<std::string>();

        if (obj.contains("categoryName") && obj["categoryName"].is_string())
            r.categoryName = obj["categoryName"].get<std::string>();

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
void RecurringManager::updateDB(const std::string& filepath,
                                DynamicArray<RecurringTransaction>& list) {
    nlohmann::json root = nlohmann::json::array();

    for (int i = 0; i < list.getSize(); i++) {
        RecurringTransaction& r = list[i];

        nlohmann::json obj;
        obj["amount"] = r.amount;
        obj["categoryId"] = r.categoryId;
        obj["categoryName"] = r.categoryName;
        obj["walletId"] = r.walletId;
        obj["description"] = r.description;
        obj["day"] = r.day;
        obj["startDate"] = r.startDate;
        obj["endDate"] = r.endDate;
        obj["lastAppliedYM"] = r.lastAppliedYM;

        root.push_back(obj);
    }

    std::ofstream file(filepath);
    if (!file.is_open()) return;

    try {
        file << root.dump(4);
    } catch (...) {
        return;
    }
}

// Main
void RecurringManager::processRecurring(IncomeManager& im, ExpenseManager& em) {
    
    // Get current YYYY-MM
    std::string currentMonth = getCurrentYM();
    
    // Get current DD
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currentDay = now->tm_mday;
    
    // Get the current YYYY-MM-DD
    std::stringstream dateSS;
    dateSS << (now->tm_year + 1900) << "-" 
           << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-"
           << std::setw(2) << std::setfill('0') << currentDay;
    std::string todayDate = dateSS.str();

    bool incomeChanged = false;
    bool expenseChanged = false;

    // Income recurring
    for (int i = 0; i < incomeRules.getSize(); ++i) {
        RecurringTransaction& rule = incomeRules[i];
        if (rule.lastAppliedYM == currentMonth) continue;
        if (currentDay < rule.day) continue;
        if (!rule.startDate.empty() && todayDate < rule.startDate) continue;
        if (!rule.endDate.empty() && todayDate > rule.endDate) continue;

        // Income profile
        Income inc;
        inc.setDate(todayDate);
        inc.setAmount(rule.amount);
        inc.setId(rule.categoryId);
        inc.setName(rule.categoryName);
        inc.setWalletId(rule.walletId);
        inc.setDescription(rule.description + " [Auto Rec]");
        
        im.add(inc); // from IncomeManager

        rule.lastAppliedYM = currentMonth;
        incomeChanged = true;
        std::cout << ">> [Auto Income] Created: " << rule.description << "\n";
    }

    // Expense recurring
    for (int i = 0; i < expenseRules.getSize(); ++i) {
        RecurringTransaction& rule = expenseRules[i];
        if (rule.lastAppliedYM == currentMonth) continue;
        if (currentDay < rule.day) continue;
        if (!rule.startDate.empty() && todayDate < rule.startDate) continue;
        if (!rule.endDate.empty() && todayDate > rule.endDate) continue;

        // Expense profile
        Expense exp;
        exp.setDate(todayDate);
        exp.setAmount(rule.amount);
        exp.setId(rule.categoryId);
        exp.setName(rule.categoryName);
        exp.setWalletId(rule.walletId);
        exp.setDescription(rule.description + " [Auto Rec]");

        em.add(exp); // from ExpenseManager

        rule.lastAppliedYM = currentMonth;
        expenseChanged = true;
        std::cout << ">> [Auto Expense] Created: " << rule.description << "\n";
    }

    // Update if there are some changes
    if (incomeChanged) updateDB(incomeFilepath, incomeRules);
    if (expenseChanged) updateDB(expenseFilepath, expenseRules);
}

