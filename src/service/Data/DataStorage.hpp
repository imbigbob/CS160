

#ifndef DATA_STORAGE_HPP
#define DATA_STORAGE_HPP
#include <json/json.h>
class DataStorage {
   public:
    enum WalletIds {
        WALLET_LIST,
        INCOME_LIST,
        EXPENSE_LIST,
        TRANSACTION_LIST,
        RECURRING_LIST
    };

   private:
    Json::Value WalletData;
    Json::Value IncomeData;
    Json::Value ExpenseData;
    Json::Value TransactionData;
    Json::Value RecurringData;

   public:
    Json::Value get_dataset(WalletIds id);
    static DataStorage& get_instance() {
        static DataStorage instance;
        return instance;
    }
    ~DataStorage();

   private:
    DataStorage();                             // Private constructor
    DataStorage(const DataStorage&) = delete;  // Delete copy constructor
    DataStorage& operator=(const DataStorage&) = delete;
};

#endif  // DATA_STORAGE_HPP