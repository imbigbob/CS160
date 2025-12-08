#include "IncomeManager.hpp"
void IncomeManager::add(const Income& w) {
    list.pushBack(w);
    return;
}

void IncomeManager::remove(int id) {}

Income* IncomeManager::findById(int id) {
    // for (int i = 0; i < list.getSize(); i++) {
    //     if (list[i].getId() == id) {
    //         return &list[i];
    //     }
    // }
    return nullptr;
}

double IncomeManager::getTotalBalance() {
    // double total = 0.0;
    // for (int i = 0; i < list.getSize(); i++) {
    //     total += list[i].getAmount();
    // }
    return 0;
}

DynamicArray<Income>& IncomeManager::getAll() { return list; }