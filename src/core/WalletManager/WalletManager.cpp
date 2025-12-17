#include "WalletManager.hpp"
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <memory>

WalletManager::WalletManager() {
    std::ifstream file("data/wallets.json");
    if (file) {
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errs;

        if (Json::parseFromStream(reader, file, &root, &errs)) {
            if (root.isArray()) {
                for (const auto& obj : root) {
                    Wallet w;
                    if (obj.isMember("id")) w.setId(obj["id"].asString());
                    if (obj.isMember("name")) w.setName(obj["name"].asString());
                    if (obj.isMember("balance")) w.setBalance(obj["balance"].asDouble());
                    list.pushBack(w);
                }
            }
        }
    }
}

void WalletManager::add(const Wallet& w) {
    list.pushBack(w);
    updateDB();
}

void WalletManager::remove(std::string id) {
    for (int i = 0; i < list.getSize(); i++) {
        if (list[i].getId() == id) {
            list.removeAt(i);
            updateDB();
            return;
        }
    }
}

Wallet* WalletManager::findById(std::string id) {
    for (int i = 0; i < list.getSize(); i++) {
        if (list[i].getId() == id) {
            return &list[i];
        }
    }
    return nullptr;
}

double WalletManager::getTotalBalance() {
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++) {
        total += list[i].getBalance();
    }
    return total;
}

DynamicArray<Wallet>& WalletManager::getAll() {
    return list;
}

void WalletManager::updateDB() {
    Json::Value root(Json::arrayValue);

    for (int i = 0; i < list.getSize(); i++) {
        const Wallet& w = list[i];
        Json::Value obj;
        obj["id"] = w.getId();
        obj["name"] = w.getName();
        obj["balance"] = w.getBalance();
        root.append(obj);
    }
    
    std::ofstream file("data/wallets.json");
    if (!file) return;

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  "; 
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}