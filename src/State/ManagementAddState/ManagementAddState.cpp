#include "ManagementAddState.hpp"
#include <iostream>
#include "../../model/Type/Type.hpp"
// Static Definitions
int ManagementAddState::sPendingMode = 0;
TypeManager *ManagementAddState::sPendingTypeManager = nullptr;

void ManagementAddState::setPayload(int mode, TypeManager *tManager)
{
    sPendingMode = mode;
    sPendingTypeManager = tManager;
}

ManagementAddState::ManagementAddState(StateStack &stack, Context context)
    : State(stack, context)
{
    // 1. Retrieve Data
    mAddingMode = sPendingMode;
    mTypeManager = sPendingTypeManager;

    sf::RenderWindow &window = *context.window;
    sf::Vector2f windowSize(window.getSize());

    // 2. Setup Background
    sf::Texture &backgroundTexture = context.textureHolder->get(Textures::ID::MenuBackground);
    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y);

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

void ManagementAddState::draw()
{
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);

    // This draws the Label, InputBox, and Buttons all at once
    window.draw(mGUIContainer);
}

bool ManagementAddState::update(sf::Time deltaTime)
{
    return true;
}

bool ManagementAddState::handleEvent(const sf::Event &event)
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

void ManagementAddState::saveTypeData()
{
    if (mTypeManager && !mInputBox->getText().empty())
    {

        std::string name = mInputBox->getText();
        Type newType(name);
        mTypeManager->addType(newType);
    }
}