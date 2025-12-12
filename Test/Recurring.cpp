#include "Recurring.h"
#include "Income.h"
#include "Expense.h"
#include "Wallet.h"
#include "WriteString.h"

Recurring_Record recurring_record;



// Add a Recurring to the Record
void addRecurringToRecord(Recurring newRecurring) {

    // Double the capacity if size reaches capacity
    if(recurring_record.size == recurring_record.capacity){

        int new_capacity = (recurring_record.capacity == 0) ? 1 : recurring_record.capacity * 2;
        Recurring *new_recd = new Recurring[new_capacity];

        // Copy old data and paste to the new house
        for(int i = 0; i < recurring_record.size; ++i){
            new_recd[i] = recurring_record.recd[i];
        }
        
        // Deleting old array and updating pointer and capacity
        delete[] recurring_record.recd;
        recurring_record.recd = new_recd;
        recurring_record.capacity = new_capacity;
    }

    // Add new recurring record
    recurring_record.recd[recurring_record.size] = newRecurring;
    recurring_record.size++;
}

// Append a Recurring to the BIN file
void appendRecurringToFile(Recurring newRecurring){
    // Open and check
    std::ofstream fout("data/RecurringRecord.bin", std::ios::binary | std::ios::app);
    if (!fout.is_open()) return;

    // Transaction data
        // Type
        fout.write(reinterpret_cast<const char*>(&newRecurring.typeTransaction), sizeof(newRecurring.typeTransaction));
        // Date
        writeString(fout, newRecurring.date);
        // ID
        fout.write(reinterpret_cast<const char*>(&newRecurring.transaction_id), sizeof(newRecurring.transaction_id));
        // Amount
        fout.write(reinterpret_cast<char*>(&newRecurring.amount), sizeof(newRecurring.amount));
        // Wallet ID
        fout.write(reinterpret_cast<const char*>(&newRecurring.wallet_id), sizeof(newRecurring.wallet_id));
        // Description
        writeString(fout, newRecurring.description);

    // Recurring data
        // ID
        fout.write(reinterpret_cast<const char*>(&newRecurring.recurring_id), sizeof(newRecurring.recurring_id));
        // Start Date
        writeString(fout, newRecurring.startDate);
        // End Date
        writeString(fout, newRecurring.endDate);
        // Last Month Applied
        fout.write(reinterpret_cast<char*>(&newRecurring.lastAppliedMonth), sizeof(newRecurring.lastAppliedMonth));

    fout.close();
}

// Edit a Recurring from the Record
void editRecurring(int reccuring_id) {
    for (int i = 0; i < recurring_record.size; ++i){
        if (recurring_record.recd[i].recurring_id == reccuring_id){

            // WIP
            return;
        }
    }    

    std::cout << "The given Recurring ID was not found.";
}

// Remove a Recurring from the Record
void removeRecurring(int recurring_id) {
    for (int i = 0; i < recurring_record.size; ++i){
        if (recurring_record.recd[i].recurring_id == recurring_id){

            // Shift elements to the left to fill the gap
            for (int j = i; j < recurring_record.size - 1; ++j){
                recurring_record.recd[j] = recurring_record.recd[j + 1];
            }
            recurring_record.size--;

            return;
        }
    }    

    std::cout << "The given Recurring ID was not found.";
}

// List all Recurrings from the Record
void listRecurringFromRecord() {
    
    std::cout << "All the current Recurrings: ";
    for (int i = 0; i < recurring_record.size; ++i) {
        std::cout << "ID: " << recurring_record.recd[i].recurring_id << "\n";

        std::cout << "Type: ";
                    if (recurring_record.recd[i].typeTransaction) {
                        std::cout << "Income" << "\n";
                    } 
                    else {
                        std::cout << "Expense" << "\n";
                    }

        std::cout << "Amount: ";
                    if (recurring_record.recd[i].typeTransaction) {
                        std::cout << "+" << recurring_record.recd[i].amount << "\n";
                    } 
                    else {
                        std::cout << "-" << recurring_record.recd[i].amount << "\n";
                    }

        if (recurring_record.recd[i].typeTransaction) {
            std::cout << "Source: " << recurring_record.recd[i].transaction_id << "\n";
        } 
        else {
            std::cout << "Category: " << recurring_record.recd[i].transaction_id << "\n";
        }

        std::cout << "Wallet: " << recurring_record.recd[i].
    }
}

// Input a Recurring from the console
void inputRecurringFromConsole() {
    Recurring newRecurring;

        // Get transaction's data
        std::cout << "Enter the type of transaction (1 for Income, 0 for Expense): ";
        std::cin >> newRecurring.typeTransaction;

        std::cout << "Enter transaction date (YYYY-MM-DD): ";
        std::cin >> newRecurring.date;

        std::cout << "Enter amount: ";
        std::cin >> newRecurring.amount;

        if (newRecurring.typeTransaction) {
            listIncomeSources();
            std::cout << "Choose income source id: ";
            std::cin >> newRecurring.transaction_id;
        }
        else {
            listExpenseCategories();
            std::cout << "Choose expense source id: ";
            std::cin >> newRecurring.transaction_id;
        }

        listWallets();
        std::cout << "Choose wallet id: ";
        std::cin >> newRecurring.wallet_id;

        std::cout << "Enter description: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, newRecurring.description);

    // Create ID from size
    newRecurring.recurring_id = recurring_record.size + 1;

    std::cout << "Please enter the start date (YYYY-MM-DD): ";
    std::cin >> newRecurring.startDate;

    std::cout << "Please enter the end date (YYYY-MM-DD) (optional = 0): ";
    std::cin >> newRecurring.endDate;
    if (newRecurring.endDate == "0") { // Optional option
        newRecurring.endDate = "";
    } 

    // Reset applied month
    newRecurring.lastAppliedMonth = 0;

    // Add new Recurring
        // to the Record
        addRecurringToRecord(newRecurring);
        // to the BIN file
        appendRecurringToFile(newRecurring);
}
