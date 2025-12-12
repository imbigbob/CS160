#ifndef RECURRINGCONFIGURATION_H
#define RECURRINGCONFIGURATION_H

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <cmath>

// Data structs and Source structs
// Records for storing data and sources (DIY vectors)
struct Recurring {
    // Income or Expense
    bool typeTransaction;
    // True = Income
    // False = Expense

        // Data of transaction
        std::string date;

        // same as source and category
        int transaction_id; // if income, then source otherwise category if expense

        double amount;

        int wallet_id;

        std::string description;

    // Data of recurring
    int recurring_id;

    std::string startDate;
    std::string endDate;

    int lastAppliedMonth;
};

struct Recurring_Record {
    Recurring* recd = nullptr;
    int size = 0;
    int capacity = 0;
};

extern Recurring_Record recurring_record;



// Functions declaration


#endif