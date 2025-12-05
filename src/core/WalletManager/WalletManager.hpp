#include "DynamicArray.hpp"
#include "model/Wallet/Wallet.hpp"
class WalletManager {
   private:
    DynamicArray<Wallet> list;

   public:
    void add(const Wallet& w);
    void remove(int id);
    Wallet* findById(int id);
    double getTotalBalance();
    DynamicArray<Wallet>& getAll();
};
