#pragma once
#ifndef TYPE_HPP
#define TYPE_HPP
#include <string>

class Type {
   private:
    std::string id;
    std::string name;

   public:
    Type() : id(""), name("") {}
    Type(std::string id, std::string name) : id(id), name(name) {}
    ~Type() {};
    std::string getId() const { return id; }
    std::string getName() const { return name; }
};
#endif  // TYPE_HPP