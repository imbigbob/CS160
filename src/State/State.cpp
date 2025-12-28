#include "State.hpp"
#include "../StateStack/StateStack.hpp"

State::Context::Context(
    sf::RenderWindow& window, 
    TextureHolder& textureHolder,
    FontHolder& fontHolder, 
    
    // Old
    IncomeManager& incomeManager,
    ExpenseManager& expenseManager,
    
    // New
    IncomeManagement& incomeManagement,
    ExpenseManagement& expenseManagement,
    WalletManager& walletManager,

    // Shared Data
    int& activeRecordType,
    int& activeRecordId
)
    : window(&window)
    , textureHolder(&textureHolder)
    , fontHolder(&fontHolder)
    
    // Old
    , incomeManager(&incomeManager)
    , expenseManager(&expenseManager)
    
    // New
    , incomeManagement(&incomeManagement)
    , expenseManagement(&expenseManagement)
    , walletManager(&walletManager)
    
    // Shared Data
    , activeRecordType(&activeRecordType)
    , activeRecordId(&activeRecordId)
{
}

State::State(StateStack& stack, Context context)
    : mStack(&stack), mContext(context) {}

State::~State() {}

void State::requestStackPush(States::ID stateID) { mStack->pushState(stateID); }

void State::requestStackPop() { mStack->popState(); }

void State::requestStackClear() { mStack->clearStates(); }

State::Context State::getContext() const { return mContext; }