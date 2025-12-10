
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

    Type type1("1", "Salary");
    Type type2("2", "Freelance");
    TypeManager typeManager("IncomeTypes");
    typeManager.addType(type1);
    typeManager.addType(type2);

        return 0;
}
