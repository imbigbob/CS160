#include "TransactionAddState.hpp"
#include <iostream>
#include "../../model/Type/Type.hpp"
#include "../WarningState/WarningState.hpp"
// Static Definitions
int TransactionAddState::sPendingMode = 0;
TransactionManager *TransactionAddState::sPendingTransactionManager = nullptr;

void TransactionAddState::setPayload(int mode, TransactionManager *tManager)
{
    sPendingMode = mode;
    sPendingTransactionManager = tManager;
}

TransactionAddState::TransactionAddState(StateStack &stack, Context context)
    : State(stack, context)
{
    // 1. Retrieve Data
    mAddingMode = sPendingMode;
    mTransactionManager = sPendingTransactionManager;

    sf::RenderWindow &window = *context.window;
    sf::Vector2f windowSize(window.getSize());

    sf::Vector2f center(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 2.f);

    const float startY = center.y - 180.f;
    const float gapY = 45.f;
    const float startX = center.x - 350.f;

    auto addField = [&](const std::string &label, float y)
    {
        auto l = std::make_shared<GUI::Label>(label, *context.fontHolder);
        l->setPosition(startX, y);
        mGUIContainer.addComponent(l);
    };

    auto addBox = [&](float y)
    {
        auto b = std::make_shared<GUI::InputBox>(*context.fontHolder);
        b->setPosition(startX + 140.f, y);
        b->setScale(1.5f, 1.5f);
        mGUIContainer.addComponent(b);
        return b;
    };

    addField("Amount:", startY);
    mAmountBox = addBox(startY);

    addField("Wallet ID:", startY + gapY);
    mWalletBox = addBox(startY + gapY);

    addField("Type ID:", startY + gapY * 2);
    mTypeBox = addBox(startY + gapY * 2);

    addField("Description:", startY + gapY * 3);
    mDescBox = addBox(startY + gapY * 3);

    auto saveBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Save");
    saveBtn->setPosition(center.x - 110.f, startY + gapY * 7 + 50.f);
    saveBtn->setCallback([this]()
                         { save(); });

    auto cancelBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Cancel");
    cancelBtn->setPosition(center.x + 110.f, startY + gapY * 7 + 50.f);
    cancelBtn->setCallback([this]()
                           { requestStackPop(); });

    mGUIContainer.addComponent(saveBtn);
    mGUIContainer.addComponent(cancelBtn);
}

void TransactionAddState::draw()
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

bool TransactionAddState::update(sf::Time deltaTime)
{
    return true;
}

bool TransactionAddState::handleEvent(const sf::Event &event)
{
    // Delegate event handling to the container
    // This handles typing in InputBox and clicking Buttons
    mGUIContainer.handleEvent(event, *getContext().window);

    // Optional: Allow 'Enter' key to save
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        save();
        requestStackPop();
    }

    return false;
}

void TransactionAddState::save()
{

    if (mAmountBox->getText().empty() ||
        mWalletBox->getText().empty() ||
        mTypeBox->getText().empty() ||
        mDescBox->getText().empty())
    {
        WarningState::setMessage("Please fill in all required fields!");
        requestStackPush(States::ID::Warning);
        return;
    }
    mTransactionManager->add(Transaction(
        "[Manual Entry]",
        std::stof(mAmountBox->getText()),
        mWalletBox->getText(),
        mTypeBox->getText(),
        mDescBox->getText()));
}
