#ifndef INCOME_MANAGER_HPP
#define INCOME_MANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Income/Income.hpp"
class IncomeManager {
   private:
    DynamicArray<Income> list;

   private:
    void updateDB();

   public:
    IncomeManager();
    void add(const Income& w);
    void remove(int id);
    Income* findById(int id);
    double getTotalBalance();
    DynamicArray<Income>& getAll();
};
#endif
// std::string Transaction::getCurrentDateTimeRaw() {
//     std::time_t now = std::time(nullptr);
//     std::tm* t = std::localtime(&now);

//     // continuous string with no formatting characters
//     std::string s = std::to_string(t->tm_year + 1900) +
//                     std::to_string(t->tm_mon + 1) +
//                     std::to_string(t->tm_mday) + std::to_string(t->tm_hour) +
//                     std::to_string(t->tm_min) + std::to_string(t->tm_sec);

//     return s;
// }