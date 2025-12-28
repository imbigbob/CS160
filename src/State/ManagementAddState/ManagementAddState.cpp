#include "ManagementAddState.hpp"
#include <iostream>
#include <string>

#include "../../model/Income/Income.hpp" 
#include "../../model/Expense/Expense.hpp" 

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
    mAddingMode = sPendingMode;
    mTypeManager = sPendingTypeManager;

    sf::RenderWindow &window = *context.window;
    sf::Vector2f windowSize(window.getSize());

    // --- LAYOUT CONSTANTS ---
    // Start higher up to fit 5 fields
    float startY = windowSize.y / 2.f - 250.f; 
    float xPos = windowSize.x / 2.f - 200.f;
    float gap = 60.f; // Space between fields

    // 1. DATE Field
    auto dateLabel = std::make_shared<GUI::Label>("Date (YYYY-MM-DD):", *context.fontHolder);
    dateLabel->setPosition(xPos, startY);
    dateLabel->setTextColor(sf::Color::Black);
    mGUIContainer.addComponent(dateLabel);

    mDateInput = std::make_shared<GUI::InputBox>(*context.fontHolder);
    mDateInput->setPosition(xPos, startY + 30.f);
    mDateInput->setScale(2.f, 2.f);
    mDateInput->activate(); // Start focus here
    mGUIContainer.addComponent(mDateInput);

    // 2. SOURCE Field
    auto sourceLabel = std::make_shared<GUI::Label>("Source Name:", *context.fontHolder);
    sourceLabel->setPosition(xPos, startY + gap * 1.5f);
    sourceLabel->setTextColor(sf::Color::Black);
    mGUIContainer.addComponent(sourceLabel);

    mSourceInput = std::make_shared<GUI::InputBox>(*context.fontHolder);
    mSourceInput->setPosition(xPos, startY + gap * 1.5f + 30.f);
    mSourceInput->setScale(2.f, 2.f);
    mGUIContainer.addComponent(mSourceInput);

    // 3. AMOUNT Field
    auto amountLabel = std::make_shared<GUI::Label>("Amount:", *context.fontHolder);
    amountLabel->setPosition(xPos, startY + gap * 3.0f);
    amountLabel->setTextColor(sf::Color::Black);
    mGUIContainer.addComponent(amountLabel);

    mAmountInput = std::make_shared<GUI::InputBox>(*context.fontHolder);
    mAmountInput->setPosition(xPos, startY + gap * 3.0f + 30.f);
    mAmountInput->setScale(2.f, 2.f);
    mGUIContainer.addComponent(mAmountInput);

    // 4. WALLET Field
    auto walletLabel = std::make_shared<GUI::Label>("Wallet Name:", *context.fontHolder);
    walletLabel->setPosition(xPos, startY + gap * 4.5f);
    walletLabel->setTextColor(sf::Color::Black);
    mGUIContainer.addComponent(walletLabel);

    mWalletInput = std::make_shared<GUI::InputBox>(*context.fontHolder);
    mWalletInput->setPosition(xPos, startY + gap * 4.5f + 30.f);
    mWalletInput->setScale(2.f, 2.f);
    mGUIContainer.addComponent(mWalletInput);

    // 5. DESCRIPTION Field
    auto descLabel = std::make_shared<GUI::Label>("Description:", *context.fontHolder);
    descLabel->setPosition(xPos, startY + gap * 6.0f);
    descLabel->setTextColor(sf::Color::Black);
    mGUIContainer.addComponent(descLabel);

    mDescInput = std::make_shared<GUI::InputBox>(*context.fontHolder);
    mDescInput->setPosition(xPos, startY + gap * 6.0f + 30.f);
    mDescInput->setScale(2.f, 2.f);
    mGUIContainer.addComponent(mDescInput);

    // --- BUTTONS ---
    float btnY = startY + gap * 8.0f; // Place buttons below everything
    
    auto saveButton = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Save");
    saveButton->setPosition(windowSize.x / 2.f - 120.f, btnY);
    saveButton->setCallback([this]()
    {
        saveTypeData();
        requestStackPop(); 
    });
    mGUIContainer.addComponent(saveButton);

    auto cancelButton = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Cancel");
    cancelButton->setPosition(windowSize.x / 2.f + 120.f, btnY);
    cancelButton->setCallback([this]()
    {
        requestStackPop(); 
    });
    mGUIContainer.addComponent(cancelButton);
}

void ManagementAddState::draw()
{
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(mGUIContainer);
}

bool ManagementAddState::update(sf::Time deltaTime)
{
    return true;
}

bool ManagementAddState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event, *getContext().window);

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        saveTypeData();
        requestStackPop();
    }
    return false;
}

void ManagementAddState::saveTypeData()
{
    // 1. Gather Data from GUI
    std::string dateStr = mDateInput->getText();
    std::string sourceStr = mSourceInput->getText();
    std::string amountStr = mAmountInput->getText();
    std::string walletStr = mWalletInput->getText();
    std::string descStr = mDescInput->getText();

    if (sourceStr.empty() || amountStr.empty()) return; 

    double amountValue = 0.0;
    try {
        amountValue = std::stod(amountStr);
    } catch (...) {
        std::cout << "Invalid Amount" << std::endl;
        return; 
    }

    // CHECK THE MODE
    // 0 = Income, 1 = Expense (Based on ManagementState enum order)
    
    if (mAddingMode == 0) // INCOME MODE
    {
        IncomeManager* incomeMgr = getContext().incomeManager;
        if (incomeMgr)
        {
            Income newIncome;
            newIncome.setDate(dateStr.empty() ? "2025-01-01" : dateStr);
            newIncome.setName(sourceStr);
            newIncome.setAmount(amountValue);
            newIncome.setWalletName(walletStr.empty() ? "Default Wallet" : walletStr);
            newIncome.setDescription(descStr);

            incomeMgr->add(newIncome); 
            std::cout << "Saved Income" << std::endl;
        }
    }
    else if (mAddingMode == 1) // EXPENSE MODE
    {
        ExpenseManager* expenseMgr = getContext().expenseManager;
        if (expenseMgr)
        {
            // Assuming your Expense class works exactly like Income
            Expense newExpense; 
            newExpense.setDate(dateStr.empty() ? "2025-01-01" : dateStr);
            newExpense.setName(sourceStr);
            newExpense.setAmount(amountValue);
            newExpense.setWalletName(walletStr.empty() ? "Default Wallet" : walletStr);
            newExpense.setDescription(descStr);

            expenseMgr->add(newExpense);
            std::cout << "Saved Expense" << std::endl;
        }
    }
}