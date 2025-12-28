#include "RecurringManager.hpp"

#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void RecurringManager::processDailyCheck(IncomeManager &incomeMgr, ExpenseManager &expenseMgr)
{
    std::string today = getCurrentDate();
    std::string currentMonth = getCurrentMonthYear();

    // ---------------------------------------------------------
    // 1. Process Recurring Incomes
    // ---------------------------------------------------------
    std::ifstream incomeFile("data/IncomeRecurring.json");
    if (incomeFile.is_open() && incomeFile.peek() != std::ifstream::traits_type::eof())
    {
        try
        {
            json root;
            incomeFile >> root;

            if (root.is_array())
            {
                for (const auto &item : root)
                {
                    std::string startDate = item.value("startDate", "");
                    std::string endDate = item.value("endDate", "");

                    // Check if today is within the active recurring period
                    if (today >= startDate && (endDate.empty() || today <= endDate))
                    {
                        std::string name = item.value("sourceName", "Recurring Income");
                        double amount = item.value("amount", 0.0);

                        // Check if we already added this specific item THIS month
                        if (!isTransactionLoggedForMonth(incomeMgr.getAll(), name, amount, currentMonth))
                        {
                            Income newInc;
                            newInc.setId(std::to_string(std::time(nullptr)) + std::to_string(rand() % 1000));
                            newInc.setDate(today);
                            newInc.setName(name);
                            newInc.setAmount(amount);
                            newInc.setDescription(item.value("description", "") + " (Auto-Recurring)");
                            newInc.setWalletId(item.value("walletId", ""));
                            
                            incomeMgr.add(newInc);
                            std::cout << "[System] Added recurring income: " << name << std::endl;
                        }
                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error processing IncomeRecurring.json: " << e.what() << std::endl;
        }
    }

    // ---------------------------------------------------------
    // 2. Process Recurring Expenses
    // ---------------------------------------------------------
    std::ifstream expenseFile("data/ExpenseRecurring.json");
    if (expenseFile.is_open() && expenseFile.peek() != std::ifstream::traits_type::eof())
    {
        try
        {
            json root;
            expenseFile >> root;

            if (root.is_array())
            {
                for (const auto &item : root)
                {
                    std::string startDate = item.value("startDate", "");
                    std::string endDate = item.value("endDate", "");

                    if (today >= startDate && (endDate.empty() || today <= endDate))
                    {
                        std::string name = item.value("categoryName", "Recurring Expense");
                        double amount = item.value("amount", 0.0);

                        if (!isTransactionLoggedForMonth(expenseMgr.getAll(), name, amount, currentMonth))
                        {
                            Expense newExp;
                            newExp.setId(std::to_string(std::time(nullptr)) + std::to_string(rand() % 1000));
                            newExp.setDate(today);
                            newExp.setName(name);
                            newExp.setAmount(amount);
                            newExp.setDescription(item.value("description", "") + " (Auto-Recurring)");
                            newExp.setWalletId(item.value("walletId", ""));
                            newExp.setWalletName(item.value("walletName", ""));

                            expenseMgr.add(newExp);
                            std::cout << "[System] Added recurring expense: " << name << std::endl;
                        }
                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error processing ExpenseRecurring.json: " << e.what() << std::endl;
        }
    }
}

// --- Manual Add Functions ---

void RecurringManager::addRecurringIncome(const std::string &sourceName, double amount, 
                                          const std::string &startDate, const std::string &endDate, 
                                          const std::string &description, const std::string &walletId,
                                          IncomeManager &incomeMgr, ExpenseManager &expenseMgr)
{
    json root = json::array();
    
    // 1. Read existing file
    std::ifstream inFile("data/IncomeRecurring.json");
    if (inFile.is_open() && inFile.peek() != std::ifstream::traits_type::eof()) {
        try {
            inFile >> root;
        } catch (...) { root = json::array(); }
    }
    inFile.close();

    // 2. Create new object
    json newObj;
    newObj["sourceName"] = sourceName;
    newObj["amount"] = amount;
    newObj["startDate"] = startDate;
    newObj["endDate"] = endDate;
    newObj["description"] = description;
    newObj["walletId"] = walletId;
    newObj["id"] = std::to_string(std::time(nullptr)); // Generate ID

    // 3. Append and Save
    root.push_back(newObj);

    std::ofstream outFile("data/IncomeRecurring.json");
    if (outFile.is_open()) {
        outFile << root.dump(4);
    }
    outFile.close();

    // 4. Run Check Immediately
    processDailyCheck(incomeMgr, expenseMgr);
}

void RecurringManager::addRecurringExpense(const std::string &categoryName, double amount, 
                                           const std::string &startDate, const std::string &endDate, 
                                           const std::string &description, const std::string &walletId, 
                                           const std::string &walletName,
                                           IncomeManager &incomeMgr, ExpenseManager &expenseMgr)
{
    json root = json::array();

    // 1. Read existing file
    std::ifstream inFile("data/ExpenseRecurring.json");
    if (inFile.is_open() && inFile.peek() != std::ifstream::traits_type::eof()) {
        try {
            inFile >> root;
        } catch (...) { root = json::array(); }
    }
    inFile.close();

    // 2. Create new object
    json newObj;
    newObj["categoryName"] = categoryName;
    newObj["amount"] = amount;
    newObj["startDate"] = startDate;
    newObj["endDate"] = endDate;
    newObj["description"] = description;
    newObj["walletId"] = walletId;
    newObj["walletName"] = walletName;
    newObj["id"] = std::to_string(std::time(nullptr));

    // 3. Append and Save
    root.push_back(newObj);

    std::ofstream outFile("data/ExpenseRecurring.json");
    if (outFile.is_open()) {
        outFile << root.dump(4);
    }
    outFile.close();

    // 4. Run Check Immediately
    processDailyCheck(incomeMgr, expenseMgr);
}

// --- Helpers ---

std::string RecurringManager::getCurrentDate()
{
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(now, "%Y-%m-%d");
    return oss.str();
}

std::string RecurringManager::getCurrentMonthYear()
{
    return getCurrentDate().substr(0, 7);
}

bool RecurringManager::isTransactionLoggedForMonth(DynamicArray<Income> &list,
                                                   const std::string &sourceName,
                                                   double amount,
                                                   const std::string &currentMonthYear)
{
    for (int i = 0; i < list.getSize(); ++i)
    {
        if (list[i].getDate().substr(0, 7) == currentMonthYear)
        {
            if (list[i].getName() == sourceName && list[i].getAmount() == amount)
            {
                return true;
            }
        }
    }
    return false;
}

bool RecurringManager::isTransactionLoggedForMonth(DynamicArray<Expense> &list,
                                                   const std::string &categoryName,
                                                   double amount,
                                                   const std::string &currentMonthYear)
{
    for (int i = 0; i < list.getSize(); ++i)
    {
        if (list[i].getDate().substr(0, 7) == currentMonthYear)
        {
            if (list[i].getName() == categoryName && list[i].getAmount() == amount)
            {
                return true;
            }
        }
    }
    return false;
}