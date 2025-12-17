#include "IncomeManager.hpp"
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

IncomeManager::IncomeManager() {
    std::ifstream file("data/incomes.json");
    if (file) {
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errs;

        if (Json::parseFromStream(reader, file, &root, &errs)) {
            if (root.isArray()) {
                for (const auto& obj : root) {
                    Income income;
                    if (obj.isMember("id")) income.setId(obj["id"].asString());
                    if (obj.isMember("date")) income.setDate(obj["date"].asString());
                    if (obj.isMember("sourceName")) income.setName(obj["sourceName"].asString());
                    if (obj.isMember("amount")) income.setAmount(obj["amount"].asDouble());
                    if (obj.isMember("walletId")) income.setWalletId(obj["walletId"].asString());
                    if (obj.isMember("walletName")) income.setWalletName(obj["walletName"].asString());
                    if (obj.isMember("description")) income.setDescription(obj["description"].asString());
                    list.pushBack(income);
                }
            }
        }
    }
    file.close(); 

    std::ifstream typeFile("data/incometypes.json");
    if (typeFile) {
        Json::Value typeRoot;
        Json::CharReaderBuilder typeReader;
        std::string typeErrs;

        if (Json::parseFromStream(typeReader, typeFile, &typeRoot, &typeErrs)) {
            if (typeRoot.isArray()) {
                for (const auto& obj : typeRoot) {
                    IncomeType type;
                    if (obj.isMember("id")) type.setId(obj["id"].asString());
                    if (obj.isMember("name")) type.setName(obj["name"].asString());
                    typeList.pushBack(type);
                }
            }
        }
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

DynamicArray<Income>& IncomeManager::getAll() { return list; }

void IncomeManager::updateDB() {
    Json::Value root(Json::arrayValue);

    for (int i = 0; i < list.getSize(); i++) {
        const Income& income = list[i];
        Json::Value obj;
        obj["id"] = income.getId();
        obj["date"] = income.getDate();
        obj["sourceName"] = income.getName();
        obj["amount"] = income.getAmount();
        obj["walletId"] = income.getWalletId();
        obj["walletName"] = income.getWalletName(); 
        obj["description"] = income.getDescription();
        root.append(obj);
    }
    
    std::ofstream file("data/incomes.json");
    if (!file) return;

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  "; 
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}


void IncomeManager::addType(const IncomeType& t) {
    typeList.pushBack(t);
    updateTypeDB();
}

void IncomeManager::modifyType(const IncomeType& t) {
    for (int i = 0; i < typeList.getSize(); i++) {
        if (typeList[i].getId() == t.getId()) {
            typeList[i] = t; 
            updateTypeDB();
            return;
        }
    }
}

void IncomeManager::removeType(std::string id) {
    for (int i = 0; i < typeList.getSize(); i++) {
        if (typeList[i].getId() == id) {
            typeList.removeAt(i);
            updateTypeDB();
            return;
        }
    }
}

DynamicArray<IncomeType>& IncomeManager::getAllTypes() {
    return typeList;
}

void IncomeManager::updateTypeDB() {
    Json::Value root(Json::arrayValue);

    for (int i = 0; i < typeList.getSize(); i++) {
        const IncomeType& type = typeList[i];
        Json::Value obj;
        obj["id"] = type.getId();
        obj["name"] = type.getName();
        root.append(obj);
    }

    std::ofstream file("data/incometypes.json");
    if (!file) return;

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}