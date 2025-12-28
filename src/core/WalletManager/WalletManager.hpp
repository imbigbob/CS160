#ifndef WALLETMANAGER_HPP
#define WALLETMANAGER_HPP

#include <string>
#include <vector>

// Data structure for Wallet
struct Wallet {
    int id;
    std::string name;
};

class WalletManager {
public:
    WalletManager();

    void addWallet(const std::string& name);
    bool editWallet(int id, const std::string& newName);
    bool deleteWallet(int id);
    
    const std::vector<Wallet>& getAll() const;
    std::string getWalletName(int id) const;

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

private:
    std::vector<Wallet> mWallets;
    int mNextId;
};

#endif