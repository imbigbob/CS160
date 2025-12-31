#include "TypeManager.hpp"

#include <nlohmann/json.hpp>
#include <cstring>
#include <string>
using json = nlohmann::json;

#include <fstream>
#include <iostream>
TypeManager::TypeManager(std::string filepath)
{
    this->filepath = filepath;
    std::ifstream fin("data/" + filepath + ".json");
    if (!fin.is_open())
    {
        types = DynamicArray<Type>();
        return;
    }

    json root;
    fin >> root;

    if (!root.is_array())
    {
        throw std::runtime_error("Invalid JSON format: root is not an array");
    }

    for (const auto &obj : root)
    {
        Type type(obj["id"].get<std::string>(), obj["name"].get<std::string>());
        types.pushBack(type);
    }
}
bool TypeManager::addType(const Type &t)
{
    for (size_t i = 0; i < types.getSize(); ++i)
    {
        if (types[i].getName().compare(t.getName()) == 0)
        {
            return false;
        }
    }
    types.pushBack(t);
    updateDb();
    return true;
}
void TypeManager::updateDb()
{
    json root = json::array();

    for (int i = 0; i < types.getSize(); i++)
    {
        const Type &type = types[i];
        json obj;
        obj["id"] = type.getId();
        obj["name"] = type.getName();

        root.push_back(obj);
    }
    // Write to file
    std::ofstream file("data/" + filepath + ".json");
    if (!file)
    {
        throw std::runtime_error(
            "Error opening " + filepath + ".json for writing");
    }

    // Pretty print with 2-space indentation
    file << root.dump(2);
}

void TypeManager::removeTypeByIndex(int index)
{
    if (index < 0 || index >= types.getSize())
        return;

    types.removeAt(index);
    updateDb();
}

void TypeManager::setNameByIndex(int index, const std::string newName)
{
    if (index < 0 || index >= types.getSize())
        return;

    types[index].setName(newName);
    updateDb();
}

bool TypeManager::isTypeNameExist(const std::string &name) const
{
    for (size_t i = 0; i < types.getSize(); ++i)
    {
        if (types[i].getName() == name)
        {
            return true;
        }
    }
    return false;
}

bool TypeManager::isTypeIdExist(const std::string &id) const
{
    for (size_t i = 0; i < types.getSize(); ++i)
    {
        if (types[i].getId() == id)
        {
            return true;
        }
    }
    return false;
}

std::string TypeManager::getTypeNameById(const std::string &id) const
{
    for (size_t i = 0; i < types.getSize(); ++i)
    {
        std::cout << "Checking type ID: " << types[i].getId() << " against " << id << std::endl;
        if (id.compare(types[i].getId()) == 0)
        {
            return types[i].getName();
        }
    }
    return "Unknown";
}