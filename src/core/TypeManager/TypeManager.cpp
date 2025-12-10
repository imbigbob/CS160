#include "TypeManager.hpp"

#include <json/json.h>

#include <fstream>
#include <iostream>
TypeManager::TypeManager(std::string filepath) {
    this->filepath = filepath;
    std::ifstream fin("data/" + filepath + ".json");
    if (!fin.is_open()) {
        types = DynamicArray<Type>();
        return;
    }

    Json::Value root;
    Json::CharReaderBuilder reader;
    std::string errs;

    if (!Json::parseFromStream(reader, fin, &root, &errs)) {
        throw std::runtime_error("Error parsing " + filepath + ": " + errs);
    }

    if (!root.isArray()) {
        throw std::runtime_error("Invalid JSON format: root is not an array");
    }

    for (const auto& obj : root) {
        Type type(obj["id"].asString(), obj["name"].asString());
        types.pushBack(type);
    }
    for (int i = 0; i < types.getSize(); i++) {
        std::cout << "Loaded type: " << types[i].getName() << std::endl;
        std::cout << "With ID: " << types[i].getId() << std::endl;
    }
}
bool TypeManager::addType(const Type& t) {
    for (size_t i = 0; i < types.getSize(); ++i) {
        if (types[i].getName().compare(t.getName()) == 0) {
            return false;
        }
    }
    types.pushBack(t);
    updateDb();
    return true;
}
void TypeManager::updateDb() {
    Json::Value root(Json::arrayValue);

    for (int i = 0; i < types.getSize(); i++) {
        const Type& type = types[i];
        Json::Value obj;
        obj["id"] = type.getId();
        obj["name"] = type.getName();

        root.append(obj);
    }
    // Write to file
    std::ofstream file("data/" + filepath + ".json");
    if (!file) {
        throw std::runtime_error(
            "Error opening " + filepath + ".json for writing"
        );
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = "  ";  // pretty print
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    jsonWriter->write(root, &file);
}