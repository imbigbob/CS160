#include "ExpenseManager.hpp"
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

ExpenseManager::ExpenseManager() {
    std::ifstream file("data/expenses.json");
    if (file) {
        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errs;

        if (Json::parseFromStream(reader, file, &root, &errs)) {
            if (root.isArray()) {
                for (const auto& obj : root) {
                    Expense expense;
                    if (obj.isMember("id")) expense.setId(obj["id"].asString());
                    if (obj.isMember("date")) expense.setDate(obj["date"].asString());
                    if (obj.isMember("categoryName")) expense.setName(obj["categoryName"].asString());
                    if (obj.isMember("amount")) expense.setAmount(obj["amount"].asDouble());
                    if (obj.isMember("walletId")) expense.setWalletId(obj["walletId"].asString());
                    if (obj.isMember("walletName")) expense.setWalletName(obj["walletName"].asString());
                    if (obj.isMember("description")) expense.setDescription(obj["description"].asString());
                    list.pushBack(expense);
                }
            }
        }
    }
    file.close(); 

    std::ifstream typeFile("data/expensetypes.json");
    if (typeFile) {
        Json::Value typeRoot;
        Json::CharReaderBuilder typeReader;
        std::string typeErrs;

        if (Json::parseFromStream(typeReader, typeFile, &typeRoot, &typeErrs)) {
            if (typeRoot.isArray()) {
                for (const auto& obj : typeRoot) {
                    ExpenseCategory cat;
                    if (obj.isMember("id")) cat.setId(obj["id"].asString());
                    if (obj.isMember("name")) cat.setName(obj["name"].asString());
                    categoryList.pushBack(cat);
                }
            }
        }
    }
}


void ExpenseManager::add(const Expense& w) {
    list.pushBack(w);
    updateDB();
}

void ExpenseManager::remove(std::string id) {
    for (int i = 0; i < list.getSize(); i++) {
        if (list[i].getId() == id) {
            list.removeAt(i);
            updateDB();
            return;
        }
    }
}

Expense* ExpenseManager::findById(std::string id) {
    for (int i = 0; i < list.getSize(); i++) {
        if (list[i].getId() == id) {
            return &list[i];
        }
    }
    return nullptr;
}

double ExpenseManager::getTotalBalance() {
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++) {
        total += list[i].getAmount();
    }
    return total;
}

DynamicArray<Expense>& ExpenseManager::getAll() { return list; }

void ExpenseManager::updateDB() {
    Json::Value root(Json::arrayValue);

    for (int i = 0; i < list.getSize(); i++) {
        const Expense& expense = list[i];
        Json::Value obj;
        obj["id"] = expense.getId();
        obj["date"] = expense.getDate();
        obj["categoryName"] = expense.getName();
        obj["amount"] = expense.getAmount();
        obj["walletId"] = expense.getWalletId();
        obj["walletName"] = expense.getWalletName(); 
        obj["description"] = expense.getDescription();
        root.append(obj);
    }
    
    std::ofstream file("data/expenses.json");
    if (!file) return;

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  "; 
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}


void ExpenseManager::addCategory(const ExpenseCategory& t) {
    categoryList.pushBack(t);
    updateCategoryDB();
}

void ExpenseManager::modifyCategory(const ExpenseCategory& t) {
    for (int i = 0; i < categoryList.getSize(); i++) {
        if (categoryList[i].getId() == t.getId()) {
            categoryList[i] = t; 
            updateCategoryDB();
            return;
        }
    }
}

void ExpenseManager::removeCategory(std::string id) {
    for (int i = 0; i < categoryList.getSize(); i++) {
        if (categoryList[i].getId() == id) {
            categoryList.removeAt(i);
            updateCategoryDB();
            return;
        }
    }
}

DynamicArray<ExpenseCategory>& ExpenseManager::getAllCategories() {
    return categoryList;
}

void ExpenseManager::updateCategoryDB() {
    Json::Value root(Json::arrayValue);

    for (int i = 0; i < categoryList.getSize(); i++) {
        const ExpenseCategory& cat = categoryList[i];
        Json::Value obj;
        obj["id"] = cat.getId();
        obj["name"] = cat.getName();
        root.append(obj);
    }

    std::ofstream file("data/expensetypes.json");
    if (!file) return;

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}