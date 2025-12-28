#include "RecurringAddState.hpp"
#include "../../core/RecurringManager/RecurringManager.hpp"
#include <iostream>

RecurringAddState::RecurringAddState(StateStack& stack, Context context)
    : State(stack, context)
    , mType(0) // Default Expense
    , mSelectedInputIndex(-1)
{
    mBackground.setTexture(context.textureHolder->get(Textures::ID::MenuBackground));

    mTitle.setFont(context.fontHolder->get(Fonts::ID::Pacifico));
    mTitle.setString("Add Recurring");
    mTitle.setCharacterSize(40);
    mTitle.setPosition(300, 30);
    
    // [FIX] Add Outline
    mTitle.setOutlineColor(sf::Color::Black);
    mTitle.setOutlineThickness(2.0f);

    mBackButton.setTexture(context.textureHolder->get(Textures::ID::ButtonBack));
    mBackButton.setPosition(50, 50);

    // --- Type Toggle Button ---
    mTypeButton.setTexture(context.textureHolder->get(Textures::ID::ButtonNormal));
    mTypeButton.setScale(0.8f, 0.8f); 
    mTypeButton.setPosition(300, 100);

    mTypeText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mTypeText.setString("Type: EXPENSE");
    mTypeText.setCharacterSize(22);
    mTypeText.setFillColor(sf::Color::Black);
    
    sf::FloatRect btnBounds = mTypeButton.getGlobalBounds();
    sf::FloatRect textBounds = mTypeText.getLocalBounds();
    mTypeText.setPosition(
        btnBounds.left + (btnBounds.width - textBounds.width) / 2.f,
        btnBounds.top + (btnBounds.height - textBounds.height) / 2.f - 5.f
    );

    // --- Inputs ---
    // [FIX] Changed last label from "WalletID" to "Wallet Name"
    std::vector<std::string> labels = {"Name/Source", "Amount", "Start (YYYY-MM-DD)", "End (Optional)", "Wallet Name"};
    float y = 170; 
    
    for (const auto& lbl : labels) {
        InputField field;
        field.stringValue = "";
        field.isSelected = false;

        field.label.setFont(context.fontHolder->get(Fonts::ID::Dosis));
        field.label.setString(lbl);
        field.label.setCharacterSize(18);
        field.label.setFillColor(sf::Color::White);
        field.label.setPosition(300, y);
        
        // Add shadow to labels for readability
        field.label.setOutlineColor(sf::Color::Black);
        field.label.setOutlineThickness(1.0f);

        field.box.setSize(sf::Vector2f(300, 30));
        field.box.setFillColor(sf::Color(20, 20, 20, 200)); 
        field.box.setOutlineThickness(2);
        field.box.setOutlineColor(sf::Color(150, 150, 150));
        field.box.setPosition(300, y + 25);

        field.value.setFont(context.fontHolder->get(Fonts::ID::Dosis));
        field.value.setString("");
        field.value.setCharacterSize(18);
        field.value.setFillColor(sf::Color::White);
        field.value.setPosition(305, y + 28);

        mInputs.push_back(field);
        y += 65; 
    }

    // --- Save Button ---
    mSaveBtn.setTexture(context.textureHolder->get(Textures::ID::ButtonNormal));
    mSaveBtn.setPosition(350, y + 10); 

    mSaveBtnText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mSaveBtnText.setString("SAVE");
    mSaveBtnText.setCharacterSize(25);
    mSaveBtnText.setFillColor(sf::Color::Black);
    
    sf::FloatRect saveBounds = mSaveBtn.getGlobalBounds();
    sf::FloatRect saveTextBounds = mSaveBtnText.getLocalBounds();
    mSaveBtnText.setPosition(
        saveBounds.left + (saveBounds.width - saveTextBounds.width) / 2.f,
        saveBounds.top + (saveBounds.height - saveTextBounds.height) / 2.f - 5.f
    );
}

void RecurringAddState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackground);
    window.draw(mBackButton);
    window.draw(mTitle);
    
    window.draw(mTypeButton);
    window.draw(mTypeText);

    for (const auto& field : mInputs) {
        window.draw(field.label);
        window.draw(field.box);
        window.draw(field.value);
    }

    window.draw(mSaveBtn);
    window.draw(mSaveBtnText);
}

bool RecurringAddState::update(sf::Time dt) { return true; }

bool RecurringAddState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        handleInput(event.text.unicode);
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

        if (mBackButton.getGlobalBounds().contains(mousePos)) {
            requestStackPop();
            return false;
        }

        if (mTypeButton.getGlobalBounds().contains(mousePos)) {
            mType = !mType;
            if (mType == 0) {
                mTypeText.setString("Type: EXPENSE");
            } else {
                mTypeText.setString("Type: INCOME");
            }
            
            sf::FloatRect btnBounds = mTypeButton.getGlobalBounds();
            sf::FloatRect textBounds = mTypeText.getLocalBounds();
            mTypeText.setPosition(
                btnBounds.left + (btnBounds.width - textBounds.width) / 2.f,
                btnBounds.top + (btnBounds.height - textBounds.height) / 2.f - 5.f
            );
        }

        mSelectedInputIndex = -1;
        for (size_t i = 0; i < mInputs.size(); ++i) {
            if (mInputs[i].box.getGlobalBounds().contains(mousePos)) {
                mSelectedInputIndex = i;
                mInputs[i].box.setOutlineColor(sf::Color::Yellow); 
            } else {
                mInputs[i].box.setOutlineColor(sf::Color(150, 150, 150));
            }
        }

        if (mSaveBtn.getGlobalBounds().contains(mousePos)) {
            saveRecurring();
            requestStackPop();
        }
    }
    return false;
}

void RecurringAddState::handleInput(sf::Uint32 unicode) {
    if (mSelectedInputIndex == -1) return;

    std::string& str = mInputs[mSelectedInputIndex].stringValue;

    if (unicode == 8) { 
        if (!str.empty()) str.pop_back();
    } else if (unicode < 128) {
        str += static_cast<char>(unicode);
    }

    mInputs[mSelectedInputIndex].value.setString(str);
}

void RecurringAddState::saveRecurring() {
    RecurringManager mgr;
    if (mInputs[0].stringValue.empty() || mInputs[1].stringValue.empty()) return;

    std::string name = mInputs[0].stringValue;
    double amount = std::atof(mInputs[1].stringValue.c_str());
    std::string start = mInputs[2].stringValue;
    std::string end = mInputs[3].stringValue;
    std::string walletName = mInputs[4].stringValue; 

    // [FIX] Passing walletName correctly. 
    // For now we pass walletName as both ID and Name to ensure it's saved somewhere.
    // In a full implementation, you'd lookup the ID from the Name.

    if (mType == 1) { // Income
        mgr.addRecurringIncome(name, amount, start, end, "Manually Added", walletName, 
            *getContext().incomeManager, *getContext().expenseManager);
    } else { // Expense
        mgr.addRecurringExpense(name, amount, start, end, "Manually Added", walletName, walletName,
            *getContext().incomeManager, *getContext().expenseManager);
    }
}