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
        if (obj.isMember("lastAppliedYM")) r.lastAppliedYM = obj["lastAppliedYM"].asString();

        list.pushBack(r);
    }
}


