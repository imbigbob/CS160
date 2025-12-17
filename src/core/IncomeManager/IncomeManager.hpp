#ifndef INCOME_MANAGER_HPP
#define INCOME_MANAGER_HPP

#include <string>
#include "core/DynamicArray/DynamicArray.hpp"

class IncomeManager {
   private:
    DynamicArray<Income> list;
    DynamicArray<IncomeType> typeList; // Master Data list

   private:
    void updateDB();      // Updates incomes.json
    void updateTypeDB();  // Updates incometypes.json

   public:
    IncomeManager();
    
    void add(const Income& w);
    void remove(std::string id);
    Income* findById(std::string id);
    double getTotalBalance();
    DynamicArray<Income>& getAll();
    
    void addType(const IncomeType& t);
    void modifyType(const IncomeType& t); 
    void removeType(std::string id);
    DynamicArray<IncomeType>& getAllTypes();
};
#endif