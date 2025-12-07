#include "DataStorage.hpp"

#include <fstream>
DataStorage::DataStorage() {
    Json::Reader reader;
    std::ifstream wallet_list_file(".\\data\\json\\wallet_list.json");
    std::ifstream income_list_file(".\\data\\json\\income_list.json");
    std::ifstream expense_list_file(".\\data\\json\\expense_list.json");
    std::ifstream transaction_list_file(".\\data\\json\\transaction_list.json");
    std::ifstream recurring_list_file(".\\data\\json\\recurring_list.json");

    reader.parse(wallet_list_file, WalletData);
    reader.parse(income_list_file, IncomeData);
    reader.parse(expense_list_file, ExpenseData);
    reader.parse(transaction_list_file, TransactionData);
    reader.parse(recurring_list_file, RecurringData);
}
Json::Value DataStorage::get_dataset(WalletIds id) {
    if (id == WALLET_LIST) {
        return WalletData;
    } else if (id == INCOME_LIST) {
        return IncomeData;
    } else if (id == EXPENSE_LIST) {
        return ExpenseData;
    } else if (id == TRANSACTION_LIST) {
        return TransactionData;
    } else if (id == RECURRING_LIST) {
        return RecurringData;
    }
    return Json::Value();
}

DataStorage::~DataStorage() {}