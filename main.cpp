
#include <json/json.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "src/Program/Program.hpp"
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
        std::cout << "EXCEPTION: " << e.what() << '\n';
    }

    // Create array of students
    std::vector<Student> students = {
        {1, "Alice", 3.8}, {2, "Bob", 3.5}, {3, "Charlie", 3.9}
    };

    Json::Value root(Json::arrayValue);

    // Convert vector<Student> → Json::Value array
    for (const auto& s : students) {
        Json::Value obj;
        obj["id"] = s.id;
        obj["name"] = s.name;
        obj["gpa"] = s.gpa;
        root.append(obj);
    }

    // Write to file
    std::ofstream file("students.json");
    if (!file) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";  // pretty print

    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);

    std::cout << "Saved to students.json\n";
    return 0;
}
