#ifndef INCOME_MANAGER_HPP
#define INCOME_MANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Income/Income.hpp"
class IncomeManager {
   private:
    DynamicArray<Income> list;

   public:
    IncomeManager() : list() {}
    void add(const Income& w);
    void remove(int id);
    Income* findById(int id);
    double getTotalBalance();
    DynamicArray<Income>& getAll();
};
#endif
