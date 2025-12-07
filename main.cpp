
#include <fstream>
#include <iostream>

#include "src/Program/Program.hpp"
int main() {
    try {
        Program program;
        program.run();
    } catch (std::exception& e) {
        std::cout << "EXCEPTION: " << e.what() << '\n';
    }
    // Json::Value dataset_Eng_Eng;
    // Json::Reader reader;
    // std::ifstream dataset_Eng_Eng_file("data/test.json ");

    // reader.parse(dataset_Eng_Eng_file, dataset_Eng_Eng);
    // std::cout << dataset_Eng_Eng;

    return 0;
}
