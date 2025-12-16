#include "IncomeManager.hpp"
#include <iostream>
#include <fstream>
#include <json/json.h> 

const std::string FILE_PATH = "data/IncomesTransaction.json";

IncomeManager::IncomeManager() {
    std::ifstream file(FILE_PATH);
    if (!file) {
       
        return;
    }

    Json::Value root;
    Json::CharReaderBuilder reader;
    std::string errs;

    if (!Json::parseFromStream(reader, file, &root, &errs)) {
        std::cerr << "Error parsing " << FILE_PATH << ": " << errs << std::endl;
        return;
    }

    if (!root.isArray()) {
        std::cerr << "Invalid JSON format: root is not an array" << std::endl;
        return;
    }

    for (const auto& obj : root) {
        Income income;
        
        if (obj.isMember("date")) income.setDate(obj["date"].asString());
        if (obj.isMember("id")) income.setId(obj["id"].asString());
        if (obj.isMember("sourceName")) income.setName(obj["sourceName"].asString());
        if (obj.isMember("amount")) income.setAmount(obj["amount"].asDouble());
        if (obj.isMember("walletId")) income.setWalletId(obj["walletId"].asString());
        if (obj.isMember("walletName")) income.setWalletName(obj["walletName"].asString());
        if (obj.isMember("description")) income.setDescription(obj["description"].asString());

        list.pushBack(income);
    }
}

void IncomeManager::add(const Income& w) {
    list.pushBack(w);
    updateDB();
}

void IncomeManager::remove(std::string id) {
    for (int i = 0; i < list.getSize(); i++) {
        if (list[i].getId() == id) {
            list.removeAt(i);
            updateDB(); 
            return;
        }
    }
    std::cout << "ID not found: " << id << std::endl;
}

Income* IncomeManager::findById(std::string id) {
    for (int i = 0; i < list.getSize(); i++) {
     
        if (list[i].getId() == id) {
            return &list[i];
        }
    }
    return nullptr;
}

double IncomeManager::getTotalBalance() {
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++) {
        total += list[i].getAmount();
    }
    return total;
}

DynamicArray<Income>& IncomeManager::getAll() {
    return list;
}

void IncomeManager::updateDB() {
    Json::Value root(Json::arrayValue);

    for (int i = 0; i < list.getSize(); i++) {
        Income& income = list[i];
        
        Json::Value obj;
        obj["date"] = income.getDate();
        obj["id"] = income.getId();
        obj["sourceName"] = income.getName(); 
        obj["amount"] = income.getAmount();
        obj["walletId"] = income.getWalletId();
        
        obj["walletName"] = income.getWalletName(); 
        
        obj["description"] = income.getDescription();

        root.append(obj);
    }

    std::ofstream file(FILE_PATH);
    if (!file) {
        std::cerr << "Error opening " << FILE_PATH << " for writing" << std::endl;
        return;
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  "; 
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}