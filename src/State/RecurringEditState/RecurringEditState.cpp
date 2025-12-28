#include "RecurringEditState.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

RecurringEditState::RecurringEditState(StateStack& stack, Context context)
    : State(stack, context)
{
    mId = *context.activeRecordId;
    mType = *context.activeRecordType; // 0 = Expense, 1 = Income

    mBackground.setTexture(context.textureHolder->get(Textures::ID::MenuBackground));
    mBackButton.setTexture(context.textureHolder->get(Textures::ID::ButtonBack));
    mBackButton.setPosition(50, 50);

    mTitle.setFont(context.fontHolder->get(Fonts::ID::Pacifico));
    mTitle.setString("Edit Recurring");
    mTitle.setCharacterSize(40);
    mTitle.setPosition(300, 50);
    
    // [FIX] Add Outline
    mTitle.setOutlineColor(sf::Color::Black);
    mTitle.setOutlineThickness(2.0f);

    // --- Info Text & Panel ---
    mInfoPanel.setFillColor(sf::Color(0, 0, 0, 150));
    mInfoPanel.setSize(sf::Vector2f(500, 200));
    mInfoPanel.setPosition(250, 140);
    mInfoPanel.setOutlineColor(sf::Color::White);
    mInfoPanel.setOutlineThickness(2);

    // --- Delete Button (ButtonNormal + Text) ---
    mDeleteBtn.setTexture(context.textureHolder->get(Textures::ID::ButtonNormal));
    mDeleteBtn.setPosition(375, 400); 

    mDeleteBtnText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mDeleteBtnText.setString("DELETE");
    mDeleteBtnText.setCharacterSize(25);
    mDeleteBtnText.setFillColor(sf::Color::Black); 

    sf::FloatRect btnBounds = mDeleteBtn.getGlobalBounds();
    sf::FloatRect textBounds = mDeleteBtnText.getLocalBounds();
    mDeleteBtnText.setPosition(
        btnBounds.left + (btnBounds.width - textBounds.width) / 2.f,
        btnBounds.top + (btnBounds.height - textBounds.height) / 2.f - 5.f
    );

    // --- Load Info Logic ---
    std::string filename = (mType == 0) ? "data/ExpenseRecurring.json" : "data/IncomeRecurring.json";
    std::ifstream file(filename);
    std::string displayText = "Loading...";
    
    if (file) {
        json root;
        try {
            file >> root;
            if (root.is_array() && mId >= 0 && mId < root.size()) {
                json obj = root[mId];
                displayText = "Type: " + std::string(mType == 0 ? "Expense" : "Income") + "\n";
                displayText += "Name: " + obj.value((mType==0?"categoryName":"sourceName"), "-") + "\n";
                displayText += "Amount: " + std::to_string(obj.value("amount", 0.0)) + "\n";
                displayText += "Start Date: " + obj.value("startDate", "-") + "\n";
                displayText += "End Date: " + obj.value("endDate", "None") + "\n";
            }
        } catch (...) { displayText = "Error loading data."; }
    }

    mInfoText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mInfoText.setString(displayText);
    mInfoText.setCharacterSize(25);
    mInfoText.setFillColor(sf::Color::White);
    mInfoText.setPosition(270, 160);
}

void RecurringEditState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackground);
    window.draw(mBackButton);
    window.draw(mTitle);
    
    window.draw(mInfoPanel);
    window.draw(mInfoText);
    
    window.draw(mDeleteBtn);
    window.draw(mDeleteBtnText);
}

bool RecurringEditState::update(sf::Time dt) { return true; }

bool RecurringEditState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

        if (mBackButton.getGlobalBounds().contains(mousePos)) {
            requestStackPop();
            return false;
        }

        if (mDeleteBtn.getGlobalBounds().contains(mousePos)) {
            deleteCurrentItem();
            requestStackPop();
        }
    }
    return false;
}

void RecurringEditState::deleteCurrentItem() {
    std::string filename = (mType == 0) ? "data/ExpenseRecurring.json" : "data/IncomeRecurring.json";
    
    std::ifstream inFile(filename);
    json root;
    if (inFile) {
        try { inFile >> root; } catch(...) { return; }
    }
    inFile.close();

    if (root.is_array() && mId >= 0 && mId < root.size()) {
        root.erase(root.begin() + mId);
        
        std::ofstream outFile(filename);
        outFile << root.dump(4);
    }
}