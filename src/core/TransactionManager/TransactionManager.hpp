#ifndef TRANSACTION_MANAGER_HPP
#define TRANSACTION_MANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Transaction/Transaction.hpp"
class TransactionManager
{
private:
    DynamicArray<Transaction> list;
    std::string filePath;

public:
    TransactionManager(std::string filePath);

    void add(const Transaction &w);
    void remove(int id);

    double getTotalBalance();
    DynamicArray<Transaction> &getAll();
    void updateDB();
    double getBalanceInTimeRange(std::string start, std::string end);
    double anualTransactionOverview(DynamicArray<int> year);
    double transactionBreakdownByType(DynamicArray<int> year, const std::string &type);
};

#endif