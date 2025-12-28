#ifndef _MANAGEMENT_ADD_STATE_HPP
#define _MANAGEMENT_ADD_STATE_HPP

#include <memory>
#include <vector>

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../GUI/InputBox/InputBox.hpp"
#include "../State.hpp"

class TransactionAddState : public State
{

private:
    // --- NEW: Static variables to hold the data temporarily ---

    static int sPendingMode;

    // Normal member variables for the instance
    int mAddingMode;

    GUI::Container mGUIContainer;

    std::shared_ptr<GUI::InputBox> mInputBox;

public:
    TransactionAddState(StateStack &stack, Context context);

    virtual void draw() override;
    virtual bool update(sf::Time deltaTime) override;
    virtual bool handleEvent(const sf::Event &event) override;
    static void setPayload(int mode, TypeManager *tManager);

private:
    void saveTypeData();
};
#endif