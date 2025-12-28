#ifndef EXPENSEMANAGEMENT_HPP
#define EXPENSEMANAGEMENT_HPP

#include <string>
#include <vector>
#include <memory>

// Data structure for an Expense Category
struct ExpenseType {
    int id;
    std::string name;
};

class ExpenseManagement {
public:
    ExpenseManagement(); // You can add (const std::string& filename) if loading JSON

    // Core Functionality
    void addExpenseType(const std::string& name);
    bool editExpenseType(int id, const std::string& newName);
    bool deleteExpenseType(int id);
    
    // Getters
    const std::vector<ExpenseType>& getAll() const;
    std::string getName(int id) const;

    // TODO: Implement load/save with your JSON library
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

private:
    std::vector<ExpenseType> mExpenseTypes;
    int mNextId; // helper to auto-increment IDs
};

#endif