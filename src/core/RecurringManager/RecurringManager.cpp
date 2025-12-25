#include "RecurringManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <json/json.h>

#include "model/Income/Income.hpp"
#include "model/Expense/Expense.hpp"

void RecurringManager::addRule(const RecurringTransaction& rule) {
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

