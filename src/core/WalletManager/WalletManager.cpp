#include "WalletManager.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string WALLET_FILE = "data/WalletTypes.json";

WalletManager::WalletManager() : mNextId(1) {
    loadFromFile(WALLET_FILE);

    if (mWallets.empty()) {
        addWallet("Cash");
        addWallet("Bank Account");
        addWallet("Credit Card");
        saveToFile(WALLET_FILE);
    }
}

void WalletManager::addWallet(const std::string& name) {
    Wallet newWallet;
    newWallet.id = mNextId++;
    newWallet.name = name;
    mWallets.push_back(newWallet);
    saveToFile(WALLET_FILE);
}

bool WalletManager::editWallet(int id, const std::string& newName) {
    for (auto& w : mWallets) {
        if (w.id == id) {
            w.name = newName;
            saveToFile(WALLET_FILE);
            return true;
        }
    }
    return false;
}

bool WalletManager::deleteWallet(int id) {
    auto it = std::remove_if(mWallets.begin(), mWallets.end(),
                             [id](const Wallet& w) { return w.id == id; });

    if (it != mWallets.end()) {
        mWallets.erase(it, mWallets.end());
        saveToFile(WALLET_FILE);
        return true;
    }
    return false;
}

const std::vector<Wallet>& WalletManager::getAll() const {
    return mWallets;
}

std::string WalletManager::getWalletName(int id) const {
    for (const auto& w : mWallets) {
        if (w.id == id) return w.name;
    }
    return "Unknown";
}

void WalletManager::saveToFile(const std::string& filename) {
    json data = json::array();
    for (const auto& w : mWallets) {
        data.push_back({{"id", w.id}, {"name", w.name}});
    }

    std::ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);
    }
}

void WalletManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    try {
        json data;
        file >> data;

        mWallets.clear();
        mNextId = 1;

        for (const auto& item : data) {
            Wallet w;
            w.id = item["id"];
            w.name = item["name"];
            mWallets.push_back(w);

            if (w.id >= mNextId) {
                mNextId = w.id + 1;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading Wallets: " << e.what() << std::endl;
    }
}