#ifndef EXPENSE_MANAGER_HPP
#define EXPENSE_MANAGER_HPP

#include <string>
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Expense/Expense.hpp"

class ExpenseManager {
   private:
    DynamicArray<Expense> list;

    // Helper to save changes to JSON
    void updateDB();

   public:
    ExpenseManager();
    
    void add(const Expense& w);
    
    // Changed int -> std::string to match Transaction ID type
    void remove(std::string id);

    // Added to find specific expenses
    Expense* findById(std::string id);

    double getTotalBalance();
    
    DynamicArray<Expense>& getAll();
};

#endif