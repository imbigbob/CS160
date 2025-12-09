#include "IncomeManager.hpp"

#include <json/json.h>

#include <fstream>
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
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++) {
        total += list[i].getAmount();
    }
    return total;
}

DynamicArray<Income>& IncomeManager::getAll() { return list; }

void IncomeManager::updateDB() {
    Json::Value root(Json::arrayValue);

    // Convert DynamicArray<Income> → Json::Value array
    for (int i = 0; i < list.getSize(); i++) {
        const Income& income = list[i];
        Json::Value obj;
        obj["id"] = income.getSourceId();
        obj["amount"] = income.getAmount();
        obj["date"] = income.getDate();
        obj["walletId"] = income.getWalletId();
        obj["description"] = income.getDescription();
        obj["sourceId"] = income.getSourceId();
        root.append(obj);
    }
    // Write to file
    std::ofstream file("incomes.json");
    if (!file) {
        throw std::runtime_error("Error opening incomes.json for writing");
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";  // pretty print
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}
