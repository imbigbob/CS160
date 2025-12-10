#ifndef TYPEMANAGER_HPP
#define TYPEMANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Type/Type.hpp"
class TypeManager {
   private:
    DynamicArray<Type> types;
    std::string filepath;

   public:
    TypeManager(std::string filepath);

    bool addType(const Type& t);
    DynamicArray<Type>& getAllTypes() { return types; }
    void updateDb();
};

#endif  // TYPEMANAGER_HPP