
#include <json/json.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "src/Program/Program.hpp"
#include "src/core/IncomeManager/IncomeManager.hpp"
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
    Income income1(1, 5000.0, "2024-06-15", 1, "Salary for June", 101);
    Income income2(2, 200.0, "2024-06-20", 1, "Freelance Project", 102);
    IncomeManager incomeManager;
    incomeManager.add(income1);
    incomeManager.add(income2);
    return 0;
}
