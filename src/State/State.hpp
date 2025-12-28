#ifndef STATE_HPP
#define STATE_HPP

#include "../ResourceHolder/ResourceHolder.hpp"

class StateStack;

// --- OLD Managers ---
class IncomeManager;
class ExpenseManager;

// --- NEW Managers ---
class IncomeManagement;
class ExpenseManagement;
class WalletManager;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context
    {
        Context(
            sf::RenderWindow &window, 
            TextureHolder &textureHolder,
            FontHolder &fontHolder, 
            
            // Old Managers
            IncomeManager &incomeManager, 
            ExpenseManager &expenseManager,
            
            // New Managers
            IncomeManagement &incomeManagement,
            ExpenseManagement &expenseManagement,
            WalletManager &walletManager,

            // Shared Data for Add/Edit
            int& activeRecordType,
            int& activeRecordId
        );

        sf::RenderWindow *window;
        TextureHolder *textureHolder;
        FontHolder *fontHolder;

        // Old Pointers
        IncomeManager *incomeManager; 
        ExpenseManager *expenseManager;

        // New Pointers
        IncomeManagement *incomeManagement;
        ExpenseManagement *expenseManagement;
        WalletManager *walletManager;

        // Shared Data Pointers
        int* activeRecordType;
        int* activeRecordId;
    };

    State(StateStack &stack, Context context);
    virtual ~State();

    virtual bool handleEvent(const sf::Event &event) = 0;
    virtual bool update(sf::Time deltaTime) = 0;
    virtual void draw() = 0;

protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStackClear();
    Context getContext() const;

private:
    StateStack *mStack;
    Context mContext;
};

#endif