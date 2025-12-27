#include "RecurringAddState.hpp"
#include <iostream>
#include "../../model/Type/Type.hpp"
// Static Definitions
int RecurringAddState::sPendingMode = 0;
TypeManager *RecurringAddState::sPendingTypeManager = nullptr;

void RecurringAddState::setPayload(int mode, TypeManager *tManager)
{
    sPendingMode = mode;
    sPendingTypeManager = tManager;
}

RecurringAddState::RecurringAddState(StateStack &stack, Context context)
    : State(stack, context)
{
    // 1. Retrieve Data
    mAddingMode = sPendingMode;
    mTypeManager = sPendingTypeManager;

    sf::RenderWindow &window = *context.window;
    sf::Vector2f windowSize(window.getSize());

    // --- 3. GUI SETUP ---

    // A. Create Label
    auto label = std::make_shared<GUI::Label>("Add Item Name:", *context.fontHolder);
    label->setScale(2.f, 2.f);
    label->setPosition(windowSize.x / 2.f - 200.f, windowSize.y / 2.f - 80.f);
    label->setTextColor(sf::Color::Black); // Ensure visibility
    mGUIContainer.addComponent(label);

    // B. Create InputBox (Using your new class)
    mInputBox = std::make_shared<GUI::InputBox>(*context.fontHolder);
    mInputBox->setScale(2.f, 2.f);
    mInputBox->setPosition(windowSize.x / 2.f - 200.f, windowSize.y / 2.f - 40.f);

    mInputBox->activate(); // Auto-focus
    mGUIContainer.addComponent(mInputBox);

    // C. Create Save Button
    auto saveButton = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Save");
    saveButton->setPosition(windowSize.x / 2.f - 120.f, windowSize.y / 2.f + 80.f);
    saveButton->setCallback([this]()
                            {
                                saveTypeData();
                                requestStackPop(); // Go back after saving
                            });
    mGUIContainer.addComponent(saveButton);

    // D. Create Cancel Button
    auto cancelButton = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Cancel");
    cancelButton->setPosition(windowSize.x / 2.f + 120.f, windowSize.y / 2.f + 80.f);
    cancelButton->setCallback([this]()
                              {
                                  requestStackPop(); // Just go back
                              });
    mGUIContainer.addComponent(cancelButton);
}

void RecurringAddState::draw()
{
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);

    // This draws the Label, InputBox, and Buttons all at once
    window.draw(mGUIContainer);
}

bool RecurringAddState::update(sf::Time deltaTime)
{
    return true;
}

bool RecurringAddState::handleEvent(const sf::Event &event)
{
    // Delegate event handling to the container
    // This handles typing in InputBox and clicking Buttons
    mGUIContainer.handleEvent(event, *getContext().window);

    // Optional: Allow 'Enter' key to save
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        saveTypeData();
        requestStackPop();
    }

    return false;
}

void RecurringAddState::saveTypeData()
{
    if (mTypeManager && !mInputBox->getText().empty())
    {

        std::string name = mInputBox->getText();
        Type newType(name);
        mTypeManager->addType(newType);
    }
}