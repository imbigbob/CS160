#ifndef INCOME_MANAGER_HPP
#define INCOME_MANAGER_HPP

#include <string>
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Income/Income.hpp"

class IncomeManager {
   private:
    DynamicArray<Income> list;

    void updateDB();

   public:
    IncomeManager();
    
    void add(const Income& w);
    
    void remove(std::string id);
    
    Income* findById(std::string id);
    
    double getTotalBalance();
    
    DynamicArray<Income>& getAll();
};

#endif