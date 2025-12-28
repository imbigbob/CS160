#include "ExpenseManagement.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string EXPENSE_FILE = "data/ExpenseTypes.json";

ExpenseManagement::ExpenseManagement() : mNextId(1) {
    loadFromFile(EXPENSE_FILE);

    if (mExpenseTypes.empty()) {
        addExpenseType("Food");
        addExpenseType("Rent");
        addExpenseType("Transportation");
        saveToFile(EXPENSE_FILE);
    }
}

void ExpenseManagement::addExpenseType(const std::string& name) {
    ExpenseType newType;
    newType.id = mNextId++;
    newType.name = name;
    mExpenseTypes.push_back(newType);
    saveToFile(EXPENSE_FILE);
}

bool ExpenseManagement::editExpenseType(int id, const std::string& newName) {
    for (auto& type : mExpenseTypes) {
        if (type.id == id) {
            type.name = newName;
            saveToFile(EXPENSE_FILE);
            return true;
        }
    }
    return false;
}

bool ExpenseManagement::deleteExpenseType(int id) {
    auto it = std::remove_if(mExpenseTypes.begin(), mExpenseTypes.end(),
                             [id](const ExpenseType& type) { return type.id == id; });

    if (it != mExpenseTypes.end()) {
        mExpenseTypes.erase(it, mExpenseTypes.end());
        saveToFile(EXPENSE_FILE);
        return true;
    }
    return false;
}

const std::vector<ExpenseType>& ExpenseManagement::getAll() const {
    return mExpenseTypes;
}

std::string ExpenseManagement::getName(int id) const {
    for (const auto& type : mExpenseTypes) {
        if (type.id == id) return type.name;
    }
    return "Unknown";
}

void ExpenseManagement::saveToFile(const std::string& filename) {
    json data = json::array();
    for (const auto& type : mExpenseTypes) {
        data.push_back({{"id", type.id}, {"name", type.name}});
    }

    std::ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);
    }
}

void ExpenseManagement::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    try {
        json data;
        file >> data;

        mExpenseTypes.clear();
        mNextId = 1;

        for (const auto& item : data) {
            ExpenseType type;
            type.id = item["id"];
            type.name = item["name"];
            mExpenseTypes.push_back(type);

            if (type.id >= mNextId) {
                mNextId = type.id + 1;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading ExpenseTypes: " << e.what() << std::endl;
    }
}