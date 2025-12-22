#include "IncomeManager.hpp"

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

#include <fstream>
IncomeManager::IncomeManager()
{
    std::ifstream file("data/IncomesTransaction.json");
    if (!file)
    {
        return;
    }

    Json root;
    file >> root;

    if (!root.is_array())
    {
        throw std::runtime_error("Invalid JSON format: root is not an array");
    }

    for (const auto &obj : root)
    {
        Income income;
        if (obj.contains("date"))
            income.setDate(obj["date"].get<std::string>());
        if (obj.contains("id"))
            income.setId(obj["id"].get<std::string>());
        if (obj.contains("sourceName"))
            income.setName(obj["sourceName"].get<std::string>());
        if (obj.contains("amount"))
            income.setAmount(obj["amount"].get<double>());
        if (obj.contains("walletId"))
            income.setWalletId(obj["walletId"].get<std::string>());

        if (obj.contains("walletName"))
            income.setWalletName(obj["walletName"].get<std::string>());
        if (obj.contains("description"))
            income.setDescription(obj["description"].get<std::string>());

        list.pushBack(income);
    }
}

void IncomeManager::add(const Income &w)
{
    list.pushBack(w);
    updateDB();
    return;
}

void IncomeManager::remove(int id) { return; }

Income *IncomeManager::findById(int id)
{
    // for (int i = 0; i < list.getSize(); i++) {
    //     if (list[i].getId() == id) {
    //         return &list[i];
    //     }
    // }
    return nullptr;
}

double IncomeManager::getTotalBalance()
{
    double total = 0.0;
    for (int i = 0; i < list.getSize(); i++)
    {
        total += list[i].getAmount();
    }
    return total;
}

DynamicArray<Income> &IncomeManager::getAll() { return list; }

void IncomeManager::updateDB()
{
    Json root = Json::array(); // JSON array root

    // Convert DynamicArray<Income> → json array
    for (int i = 0; i < list.getSize(); i++)
    {
        const Income &income = list[i];

        Json obj;
        obj["date"] = income.getDate();
        obj["id"] = income.getId();
        obj["sourceName"] = income.getName();
        obj["amount"] = income.getAmount();
        obj["walletId"] = income.getWalletId();
        obj["walletName"] = ""; // placeholder
        obj["description"] = income.getDescription();

        root.push_back(obj);
    }

    // Write to file
    std::ofstream file("data/incomes.json");
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening data/incomes.json for writing");
    }

    // Pretty print with 2-space indentation
    file << root.dump(2);
}
