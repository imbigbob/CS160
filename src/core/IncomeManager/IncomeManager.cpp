#include "IncomeManager.hpp"

#include <json/json.h>

#include <fstream>
IncomeManager::IncomeManager() {
    std::ifstream file("data/incomes.json");
    if (!file) {
        return;
    }

    Json::Value root;
    Json::CharReaderBuilder reader;
    std::string errs;

    if (!Json::parseFromStream(reader, file, &root, &errs)) {
        throw std::runtime_error("Error parsing incomes.json: " + errs);
    }

    if (!root.isArray()) {
        throw std::runtime_error("Invalid JSON format: root is not an array");
    }

    for (const auto& obj : root) {
        Income income;
        if (obj.isMember("date")) income.setDate(obj["date"].asString());
        if (obj.isMember("id")) income.setId(obj["id"].asString());
        if (obj.isMember("sourceName"))
            income.setName(obj["sourceName"].asString());
        if (obj.isMember("amount")) income.setAmount(obj["amount"].asDouble());

        if (obj.isMember("walletId"))
            income.setWalletId(obj["walletId"].asString());

        if (obj.isMember("walletName"))
            income.setWalletName(obj["walletName"].asString());
        if (obj.isMember("description"))
            income.setDescription(obj["description"].asString());

        list.pushBack(income);
    }
}

void IncomeManager::add(const Income& w) {
    list.pushBack(w);
    updateDB();
    return;
}

void IncomeManager::remove(int id) { return; }

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
        obj["date"] = income.getDate();
        obj["id"] = income.getId();
        obj["sourceName"] = income.getName();
        obj["amount"] = income.getAmount();

        obj["walletId"] = income.getWalletId();
        obj["walletName"] = "";  // Placeholder, as Wallet name is not in Income
        obj["description"] = income.getDescription();

        root.append(obj);
    }
    // Write to file
    std::ofstream file("data/incomes.json");
    if (!file) {
        throw std::runtime_error("Error opening incomes.json for writing");
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";  // pretty print
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}
