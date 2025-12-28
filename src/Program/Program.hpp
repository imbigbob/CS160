#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <SFML/Graphics.hpp>

#include "../ResourceHolder/ResourceHolder.hpp"
#include "../StateStack/StateStack.hpp"

#include "../core/IncomeManager/IncomeManager.hpp"
#include "../core/ExpenseManager/ExpenseManager.hpp"

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
    StateStack mStateStack;

    IncomeManager mIncomeManager;
    ExpenseManager mExpenseManager;
};

#endif