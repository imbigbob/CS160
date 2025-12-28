#include "MasterManagementAddState.hpp"
#include "../GUI/Button/Button.hpp"
#include "../../core/IncomeManagement/IncomeManagement.hpp"
#include "../../core/ExpenseManagement/ExpenseManagement.hpp"
#include "../../core/WalletManager/WalletManager.hpp"

MasterManagementAddState::MasterManagementAddState(StateStack& stack, Context context)
    : State(stack, context) 
{
    sf::Vector2f windowSize(context.window->getSize());

    // Background
    mBackgroundSprite.setTexture(context.textureHolder->get(Textures::ID::MenuBackground));
    mBackgroundSprite.setScale(windowSize.x / mBackgroundSprite.getLocalBounds().width, windowSize.y / mBackgroundSprite.getLocalBounds().height);

    // Identify what we are adding
    int type = *getContext().activeRecordType;
    std::string titleString = "Add New ";
    if (type == 0) titleString += "Expense Category";
    else if (type == 1) titleString += "Income Source";
    else titleString += "Wallet";

    // Title
    mTitleText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mTitleText.setString(titleString);
    mTitleText.setCharacterSize(40);
    mTitleText.setPosition(windowSize.x / 2.f - mTitleText.getLocalBounds().width / 2.f, 100.f);

    // Input Box Visuals
    mInputBox.setSize(sf::Vector2f(400.f, 50.f));
    mInputBox.setFillColor(sf::Color(255, 255, 255, 200));
    mInputBox.setPosition(windowSize.x / 2.f - 200.f, 200.f);
    mInputBox.setOutlineThickness(2.f);
    mInputBox.setOutlineColor(sf::Color::White);

    mInputText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mInputText.setString("|"); // Cursor
    mInputText.setCharacterSize(30);
    mInputText.setFillColor(sf::Color::Black);
    mInputText.setPosition(mInputBox.getPosition().x + 10.f, mInputBox.getPosition().y + 5.f);

    // Save Button
    auto saveBtn = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Save");
    saveBtn->setPosition(windowSize.x / 2.f - 100.f, 300.f);
    saveBtn->setCallback([this, type]() {
        if (mCurrentInput.empty()) return; // Don't save empty strings

        // Logic to Save
        if (type == 0) getContext().expenseManagement->addExpenseType(mCurrentInput);
        else if (type == 1) getContext().incomeManagement->addIncomeType(mCurrentInput);
        else if (type == 2) getContext().walletManager->addWallet(mCurrentInput);

        requestStackPop(); // Go back to list
    });

    // Cancel Button
    auto cancelBtn = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Cancel");
    cancelBtn->setPosition(windowSize.x / 2.f + 50.f, 300.f);
    cancelBtn->setCallback([this]() { requestStackPop(); });

    mGUIContainer.addComponent(saveBtn);
    mGUIContainer.addComponent(cancelBtn);
}

bool MasterManagementAddState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);

    // Basic Text Input Handling
    if (event.type == sf::Event::TextEntered) {
        // Handle Backspace (ASCII 8)
        if (event.text.unicode == 8) {
            if (!mCurrentInput.empty()) mCurrentInput.pop_back();
        }
        // Handle printable characters (A-Z, 0-9, space, etc.)
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            mCurrentInput += static_cast<char>(event.text.unicode);
        }
        mInputText.setString(mCurrentInput + "|");
    }
    return false;
}

bool MasterManagementAddState::update(sf::Time deltaTime) { return true; }

void MasterManagementAddState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    window.draw(mTitleText);
    window.draw(mInputBox);
    window.draw(mInputText);
    window.draw(mGUIContainer);
}