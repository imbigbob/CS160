#ifndef TYPEMANAGER_HPP
#define TYPEMANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Type/Type.hpp"
class TypeManager
{
private:
    DynamicArray<Type> types;
    std::string filepath;

public:
    TypeManager(std::string filepath);

    bool addType(const Type &t);
    // void setNameByIndex(int index, const std::string &newName);
    DynamicArray<Type> &getAllTypes() { return types; }
    void removeTypeByIndex(int index);
    void updateDb();
    void setNameByIndex(int index, const std::string newName);
    bool isTypeNameExist(const std::string &name) const;
    bool isTypeIdExist(const std::string &id) const;
    std::string getTypeNameById(const std::string &id) const;
};

#endif // TYPEMANAGER_HPP