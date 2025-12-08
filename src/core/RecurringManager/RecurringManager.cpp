#include "RecurringManager.hpp"
void RecurringManager::addRule(const RecurringTransaction& rule) {
    rules.pushBack(rule);
}

void RecurringManager::applyForCurrentMonth(TransactionManager& tm) {
    // Placeholder implementation
    return;
}