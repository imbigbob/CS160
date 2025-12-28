#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <SFML/Graphics.hpp>
#include "../ResourceHolder/ResourceHolder.hpp"
#include "../StateStack/StateStack.hpp"

// --- OLD Managers ---
#include "../core/IncomeManager/IncomeManager.hpp"
#include "../core/ExpenseManager/ExpenseManager.hpp"

// --- NEW Managers ---
// Ensure these paths match your actual directory structure
#include "../core/IncomeManagement/IncomeManagement.hpp"
#include "../core/ExpenseManagement/ExpenseManagement.hpp"
#include "../core/WalletManager/WalletManager.hpp"

class Program
{
public:
    Program();
    ~Program();
    void run();

private:
    void handleEvent(sf::Event &event);
    void update();
    void draw();
    void loadTextures();
    void loadFonts();
    void registerStates();

private:
    static const sf::Time TIME_PER_FRAME;

    sf::RenderWindow mWindow;
    TextureHolder mTextureHolder;
    FontHolder mFontHolder;
    
    // We must declare managers BEFORE StateStack so they are initialized first
    
    // Old
    IncomeManager mIncomeManager;
    ExpenseManager mExpenseManager;

    // New
    IncomeManagement mIncomeManagement;
    ExpenseManagement mExpenseManagement;
    WalletManager mWalletManager;

    int mActiveRecordType; // 0=Expense, 1=Income, 2=Wallet
    int mActiveRecordId;   // ID of the item being edited

    StateStack mStateStack;
};

#endif