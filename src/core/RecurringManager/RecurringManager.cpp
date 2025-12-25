#include "RecurringManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <json/json.h>

#include "model/Income/Income.hpp"
#include "model/Expense/Expense.hpp"

// File path
RecurringManager::RecurringManager() {
    this->incomeFilepath = "data/RecurringIncomes.json";
    this->expenseFilepath = "data/RecurringExpenses.json";

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
        }
        case 2: {
            expenseRules.pushBack(rule);
            updateDB(incomeFilepath, expenseRules);           
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
void RecurringManager::loadFromDB(const std::string& filepath, DynamicArray<RecurringTransaction>& list, int typeTransaction) {

    // Json read stuff
    std::ifstream file(filepath);
    if (!file.is_open()) return;

    Json::Value root;
    Json::CharReaderBuilder reader;
    std::string errs;

    if (!Json::parseFromStream(reader, file, &root, &errs)) return;

    for (const auto& obj : root) {

        // Define type of transaction
        RecurringTransaction r;
        r.type = typeTransaction; 

        if (obj.isMember("amount")) r.amount = obj["amount"].asDouble();
        if (obj.isMember("categoryId")) r.categoryId = obj["categoryId"].asString();
        if (obj.isMember("categoryName")) r.categoryName = obj["categoryName"].asString();
        if (obj.isMember("walletId")) r.walletId = obj["walletId"].asString();
        if (obj.isMember("description")) r.description = obj["description"].asString();
        if (obj.isMember("day")) r.day = obj["day"].asInt();
        if (obj.isMember("startDate")) r.startDate = obj["startDate"].asString();
        if (obj.isMember("endDate")) r.endDate = obj["endDate"].asString();
        if (obj.isMember("lastAppliedYM")) r.lastAppliedYM = obj["lastAppliedYM"].asString();

        list.pushBack(r);
    }

    file.close();
}

// Write to json file
void RecurringManager::updateDB(const std::string& filepath, DynamicArray<RecurringTransaction>& list) {

    // Json write stuff
    std::ofstream file(filepath);
    if (!file.is_open()) return;

    Json::Value root(Json::arrayValue);

    for (int i = 0; i < list.getSize(); i++) {
        RecurringTransaction& r = list[i];
        Json::Value obj;

        obj["amount"] = r.amount;
        obj["categoryId"] = r.categoryId;
        obj["categoryName"] = r.categoryName;
        obj["walletId"] = r.walletId;
        obj["description"] = r.description;
        obj["day"] = r.day;
        obj["startDate"] = r.startDate;
        obj["endDate"] = r.endDate;
        obj["lastAppliedYM"] = r.lastAppliedYM;

        root.append(obj);
    }

    file << root;
    file.close();
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

