#ifndef INCOME_MANAGER_HPP
#define INCOME_MANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Income/Income.hpp"
class IncomeManager
{
private:
    DynamicArray<Income> list;

private:
    void updateDB();

public:
    IncomeManager();
    void add(const Income &w);
    void remove(int id);
    Income *findById(int id);
    double getTotalBalance();
    DynamicArray<Income> &getAll();
    double getBalanceInTimeRange(std::string start, std::string end);
    double anualIncomeOverview(DynamicArray<int> year);
    double incomeBreakdownBySource(DynamicArray<int> year, const std::string &type);
};
#endif