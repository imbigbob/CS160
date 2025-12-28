// src/State/State.cpp
#include "State.hpp"

#include "../StateStack/StateStack.hpp"

// Update the constructor definition
State::Context::Context(
    sf::RenderWindow& window, TextureHolder& textureHolder,
    FontHolder& fontHolder, 
    IncomeManager& incomeManager,
    ExpenseManager& expenseManager // <--- Add Parameter
)
    : window(&window), 
      textureHolder(&textureHolder), 
      fontHolder(&fontHolder),
      incomeManager(&incomeManager),
      expenseManager(&expenseManager) // <--- Initialize it
{
}

State::State(StateStack& stack, Context context)
    : mStack(&stack), mContext(context) {}

State::~State() {}

void State::requestStackPush(States::ID stateID) { mStack->pushState(stateID); }

void State::requestStackPop() { mStack->popState(); }

void State::requestStackClear() { mStack->clearStates(); }

State::Context State::getContext() const { return mContext; }