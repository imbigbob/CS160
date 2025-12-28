#include "IncomeManagement.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string INCOME_FILE = "data/IncomeTypes.json";

// Updated Constructor Name
IncomeManagement::IncomeManagement() : mNextId(1) {
    loadFromFile(INCOME_FILE);
    
    if (mIncomeTypes.empty()) {
        addIncomeType("X");
        addIncomeType("Y");
        addIncomeType("Z");
        saveToFile(INCOME_FILE);
    }
}

// Updated Method Scopes
void IncomeManagement::addIncomeType(const std::string& name) {
    IncomeType newType;
    newType.id = mNextId++;
    newType.name = name;
    mIncomeTypes.push_back(newType);
    saveToFile(INCOME_FILE);
}

bool IncomeManagement::editIncomeType(int id, const std::string& newName) {
    for (auto& type : mIncomeTypes) {
        if (type.id == id) {
            type.name = newName;
            saveToFile(INCOME_FILE);
            return true;
        }
    }
    return false;
}

bool IncomeManagement::deleteIncomeType(int id) {
    auto it = std::remove_if(mIncomeTypes.begin(), mIncomeTypes.end(),
                             [id](const IncomeType& type) { return type.id == id; });

    if (it != mIncomeTypes.end()) {
        mIncomeTypes.erase(it, mIncomeTypes.end());
        saveToFile(INCOME_FILE);
        return true;
    }
    return false;
}

const std::vector<IncomeType>& IncomeManagement::getAll() const {
    return mIncomeTypes;
}

void IncomeManagement::saveToFile(const std::string& filename) {
    json data = json::array();
    for (const auto& type : mIncomeTypes) {
        data.push_back({{"id", type.id}, {"name", type.name}});
    }

    std::ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);
    }
}

void IncomeManagement::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    try {
        json data;
        file >> data;

        mIncomeTypes.clear();
        mNextId = 1; 

        for (const auto& item : data) {
            IncomeType type;
            type.id = item["id"];
            type.name = item["name"];
            mIncomeTypes.push_back(type);

            if (type.id >= mNextId) {
                mNextId = type.id + 1;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading IncomeTypes: " << e.what() << std::endl;
    }
}