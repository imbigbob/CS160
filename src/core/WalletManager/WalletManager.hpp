#ifndef WALLETMANAGER_HPP
#define WALLETMANAGER_HPP
#include "core/DynamicArray/DynamicArray.hpp"
#include "model/Wallet/Wallet.hpp"
class WalletManager
{
    DynamicArray<Wallet> wallets;

public:
    WalletManager();
    bool addWallet(const Wallet &wallet);
    // void setNameByIndex(int index, const std::string &newName);
    const DynamicArray<Wallet> &getAllWallets() { return wallets; }
    void removeWalletByIndex(int index);
    void updateDb();
    void setNameByIndex(int index, const std::string newName);
    Wallet *getWalletByIndex(const std::string &id)
    {
        for (int i = 0; i < wallets.getSize(); i++)
        {
            if (wallets[i].getId() == id)
            {
                return &wallets[i];
            }
        }
        return nullptr;
    }
};

#endif