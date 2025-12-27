#ifndef WARNINGSTATE_HPP
#define WARNINGSTATE_HPP
#include "../GUI/Container/Container.hpp"
#include "../State.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Label/Label.hpp"

#include <string>

class WarningState : public State
{
public:
    static void setMessage(const std::string &msg);

    WarningState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time) override;
    virtual bool handleEvent(const sf::Event &) override;

private:
    static std::string sMessage;

    sf::Text mMessageText;
    GUI::Container mGUIContainer;
};

#endif // WARNINGSTATE_HPP