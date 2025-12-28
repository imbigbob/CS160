#ifndef STATE_HPP
#define STATE_HPP

#include "../ResourceHolder/ResourceHolder.hpp"

class StateStack;
class IncomeManager; 
class ExpenseManager;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context
    {
        Context(
            sf::RenderWindow &window, TextureHolder &textureHolder,
            FontHolder &fontHolder, 
            IncomeManager &incomeManager, 
            ExpenseManager &expenseManager); // <--- Add Parameter

        sf::RenderWindow *window;
        TextureHolder *textureHolder;
        FontHolder *fontHolder;
        IncomeManager *incomeManager; 
        ExpenseManager *expenseManager;
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
