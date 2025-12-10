
#include <json/json.h>

#include <fstream>
#include <iostream>
#include <string>

#include "src/Program/Program.hpp"
#include "src/core/TypeManager/TypeManager.hpp"
#include "src/model/Type/Type.hpp"
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

    // Type type1("1", "Salary");
    // Type type2("2", "Freelance");
    // TypeManager typeManager("IncomeTypes");
    // typeManager.addType(type1);
    // typeManager.addType(type2);

    // Type type3("1", "Rent");
    // Type type4("2", "Utilities");
    // TypeManager expenseTypeManager("ExpenseTypes");
    // expenseTypeManager.addType(type3);
    // expenseTypeManager.addType(type4);

    // Type type5("1", "Credit Card");
    // Type type6("2", "Personal Loan");
    // TypeManager walletTypeManager("WalletTypes");
    // walletTypeManager.addType(type5);
    // walletTypeManager.addType(type6);
    return 0;
}
