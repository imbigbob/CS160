#pragma once
#ifndef TYPE_HPP
#define TYPE_HPP
#include <string>
#include <ctime>
class Type
{
private:
    std::string id;
    std::string name;
    std::string formatId()
    {
        // Simple example: use current timestamp as ID
        std::time_t now = std::time(nullptr);
        return std::to_string(now);
    }

public:
    Type() : id(""), name("") {}
    Type(std::string name) : name(name) { id = formatId(); }
    Type(const std::string &id, const std::string &name) : id(id), name(name) {}
    ~Type() {};
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    void setName(const std::string &newName) { name = newName; }
};
#endif // TYPE_HPP