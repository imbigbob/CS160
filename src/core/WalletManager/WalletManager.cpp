#include "WalletManager.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <fstream>
#include <iostream>
WalletManager::WalletManager()
{
    std::ifstream fin("data/WalletTypes.json");
    if (!fin.is_open())
    {
        wallets = DynamicArray<Wallet>();
        return;
    }

    json root;
    fin >> root;

    if (!root.is_array())
    {
        throw std::runtime_error("Invalid JSON format: root is not an array");
    }

    for (const auto &obj : root)
    {
        Wallet wallet(obj["id"].get<std::string>(), obj["name"].get<std::string>(), obj["balance"].get<double>());
        wallets.pushBack(wallet);
    }
}
bool WalletManager::addWallet(const Wallet &t)
{
    for (size_t i = 0; i < wallets.getSize(); ++i)
    {
        if (wallets[i].name.compare(t.name) == 0)
        {
            return false;
        }
    }
    wallets.pushBack(t);
    updateDb();
    return true;
}
void WalletManager::updateDb()
{
    json root = json::array();

    for (int i = 0; i < wallets.getSize(); i++)
    {
        const Wallet &wallet = wallets[i];
        json obj;
        obj["id"] = wallet.id;
        obj["name"] = wallet.name;
        obj["balance"] = wallet.balance;

        root.push_back(obj);
    }
    // Write to file
    std::ofstream file("data/WalletTypes.json");
    if (!file)
    {
        throw std::runtime_error(
            "Error opening WalletTypes.json for writing");
    }

    // Pretty print with 2-space indentation
    file << root.dump(2);
}

void WalletManager::removeWalletByIndex(int index)
{
    if (index < 0 || index >= wallets.getSize())
        return;

    wallets.removeAt(index);
    updateDb();
}

void WalletManager::setNameByIndex(int index, const std::string newName)
{
    if (index < 0 || index >= wallets.getSize())
        return;

    wallets[index].setName(newName);
    updateDb();
}