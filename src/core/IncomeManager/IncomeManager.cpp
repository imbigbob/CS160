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
    std::ofstream file("data/IncomesTransaction.json");
    if (!file.is_open())
    {
        throw std::runtime_error("Error opening data/IncomesTransaction.json for writing");
    }

    // Pretty print with 2-space indentation
    file << root.dump(2);
}
double IncomeManager::getBalanceInTimeRange(std::string start, std::string end)
{
    double totalBalance = 0.0;

    // Loop through the DynamicArray
    for (int i = 0; i < list.getSize(); ++i)
    {
        // 1. Get the current income object
        const Income &currentIncome = list[i];

        // 2. Get the date string (e.g., "2024-06-01")
        std::string incomeDate = currentIncome.getDate();

        // 3. String comparison works for YYYY-MM-DD format
        // Check if date is >= start AND <= end
        if (incomeDate >= start && incomeDate <= end)
        {
            totalBalance += currentIncome.getAmount();
        }
    }

    // Return as int (or change function signature to return double)
    return totalBalance;
}

double IncomeManager::anualIncomeOverview(DynamicArray<int> year)
{
    double totalIncome = 0.0;

    // 1. Loop through the list of years you want to check (e.g., 2023, 2024)
    for (int i = 0; i < year.getSize(); ++i)
    {
        int targetYear = year[i];
        std::string targetYearStr = std::to_string(targetYear);

        // 2. Loop through all income entries to find matches
        for (int j = 0; j < list.getSize(); ++j)
        {
            std::string incomeDate = list[j].getDate(); // Returns "YYYY-MM-DD"

            // 3. Check if the date has enough characters and matches the year
            // substr(0, 4) extracts the first 4 chars (The Year)
            if (incomeDate.length() >= 4 && incomeDate.substr(0, 4) == targetYearStr)
            {
                totalIncome += list[j].getAmount();
            }
        }
    }

    return totalIncome;
}

double IncomeManager::incomeBreakdownBySource(DynamicArray<int> year, const std::string &sourceName)
{
    double totalIncome = 0.0;

    // 1. Loop through the list of years you want to check (e.g., 2023, 2024)
    for (int i = 0; i < year.getSize(); ++i)
    {
        int targetYear = year[i];
        std::string targetYearStr = std::to_string(targetYear);

        // 2. Loop through all income entries to find matches
        for (int j = 0; j < list.getSize(); ++j)
        {
            std::string incomeDate = list[j].getDate(); // Returns "YYYY-MM-DD"
            std::string incomeSourceName = list[j].getName();

            // 3. Check if the date has enough characters and matches the year
            // substr(0, 4) extracts the first 4 chars (The Year)
            if (incomeDate.length() >= 4 && incomeDate.substr(0, 4) == targetYearStr &&
                incomeSourceName == sourceName)
            {
                totalIncome += list[j].getAmount();
            }
        }
    }

    return totalIncome;
}