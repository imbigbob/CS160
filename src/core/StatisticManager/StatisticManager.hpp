#ifndef STATISTICMANAGER_HPP
#define STATISTICMANAGER_HPP

#include <string>
#include <vector>

#include "core/ExpenseManager/ExpenseManager.hpp"
#include "core/IncomeManager/IncomeManager.hpp"
class StatisticManager {
   private:
    IncomeManager incomeManager;
    ExpenseManager expenseManager;
    std::string startDate;  // dd/mm/yyyy
    std::string endDate;    // dd/mm/yyyy

   public:
    StatisticManager(
        IncomeManager& im, ExpenseManager& em
    );  // assign the date open app or the first date in this month
    double getTotalIncome();

    double getTotalExpense();
    double getNetBalance();
    double getIncomeInTimeRange(
        const std::string& start, const std::string& end
    );
    double getExpenseInTimeRange(
        const std::string& start, const std::string& end
    );
    double getNetBalanceInTimeRange(
        const std::string& start, const std::string& end
    );
    double annualIncomeOverview(std::vector<int> year);
    double annualExpenseOverview(std::vector<int> year);
    double annualIncomeBreakdownBySource(
        std::vector<int> year, const std::string& sourceName
    );
    double annualExpenseBreakdownByCategory(
        std::vector<int> year, const std::string& categoryName
    );
};
#endif