#include "DynamicArray.hpp"
#include "model/Expense/Expense.hpp"
class ExpenseManager {
   private:
    DynamicArray<Expense> list;

   public:
    void add(const Expense& w);
    void remove(int id);
    Expense* findById(int id);
    double getTotalBalance();
    DynamicArray<Expense>& getAll();
};
