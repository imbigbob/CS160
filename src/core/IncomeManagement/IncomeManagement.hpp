#ifndef INCOMEMANAGEMENT_HPP
#define INCOMEMANAGEMENT_HPP

#include <string>
#include <vector>

struct IncomeType {
    int id;
    std::string name;
};

// Renamed class to IncomeManagement to avoid conflict
class IncomeManagement { 
public:
    IncomeManagement(); 

    void addIncomeType(const std::string& name);
    bool editIncomeType(int id, const std::string& newName);
    bool deleteIncomeType(int id);
    
    const std::vector<IncomeType>& getAll() const;

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

private:
    std::vector<IncomeType> mIncomeTypes;
    int mNextId;
};

#endif