
#include <json/json.h>

#include <fstream>
#include <iostream>
#include <string>

#include "src/Program/Program.hpp"
#include "src/core/ExpenseManager/ExpenseManager.hpp"
#include "src/core/IncomeManager/IncomeManager.hpp"
#include "src/model/Expense/Expense.hpp"
#include "src/model/Income/Income.hpp"
struct Student {
    int id;
    std::string name;
    double gpa;
};

int main() {
    try {
        Program program;
        program.run();
    } catch (std::exception& e) {
        std::cout << "EXCEPTION bug: " << e.what() << '\n';
    }

    // IncomeManager incomeManager;

    // std::cout << "Total Income: " << incomeManager.getTotalBalance()
    //           << std::endl;
    Expense expense(
        "2024-10-01", "1", "Groceries", 150.0, "W1", "Main Wallet",
        "Weekly groceries"
    );
    Expense expense2(
        "2024-10-02", "2", "Transport", 50.0, "W1", "Main Wallet", "Bus pass"
    );
    Expense expense3(
        "2024-10-03", "3", "Dining Out", 80.0, "W1", "Cash Wallet",
        "Dinner with friends"
    );
    ExpenseManager expenseManager;
    expenseManager.add(expense);
    expenseManager.add(expense2);
    expenseManager.add(expense3);
    // expenseManager.updateDB();

    return 0;
}
