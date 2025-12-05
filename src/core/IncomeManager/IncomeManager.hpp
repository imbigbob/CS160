#include "DynamicArray.hpp"
#include "model/Income/Income.hpp"
class IncomeManager {
   private:
    DynamicArray<Income> list;

   public:
    void add(const Income& w);
    void remove(int id);
    Income* findById(int id);
    double getTotalBalance();
    DynamicArray<Income>& getAll();
};
