#include "RecurringState.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

RecurringState::RecurringState(StateStack& stack, Context context)
    : State(stack, context)
{
    // Background
    mBackground.setTexture(context.textureHolder->get(Textures::ID::MenuBackground));

    // Title
    mTitle.setFont(context.fontHolder->get(Fonts::ID::Pacifico));
    mTitle.setString("Recurring Transactions");
    mTitle.setCharacterSize(40);
    mTitle.setPosition(300, 50);
    
    // [FIX] Strong Outline
    mTitle.setOutlineColor(sf::Color::Black);
    mTitle.setOutlineThickness(2.5f);

    // Add Button
    mAddButtonBg.setSize(sf::Vector2f(150, 40));
    mAddButtonBg.setFillColor(sf::Color(100, 100, 250));
    mAddButtonBg.setPosition(800, 50);

    mAddButtonText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mAddButtonText.setString("Add New");
    mAddButtonText.setCharacterSize(20);
    mAddButtonText.setFillColor(sf::Color::White);
    mAddButtonText.setPosition(835, 58);
    
    // [FIX] Strong Outline for Button Text
    mAddButtonText.setOutlineColor(sf::Color::Black);
    mAddButtonText.setOutlineThickness(2.0f);

    // Back Button
    mBackButton.setTexture(context.textureHolder->get(Textures::ID::ButtonBack));
    mBackButton.setPosition(50, 50);

    loadRecurringData();
}

void RecurringState::loadRecurringData() {
    mItems.clear();
    float yOffset = 150;
    
    // 1. Load Expenses (Type 0)
    std::ifstream expFile("data/ExpenseRecurring.json");
    if (expFile && expFile.peek() != std::ifstream::traits_type::eof()) {
        json root;
        try { expFile >> root; } catch (...) {}
        
        int index = 0;
        for (const auto& item : root) {
            RecurringItem ri;
            ri.id = index++;
            ri.type = 0; 

            std::string name = item.value("categoryName", "Unknown");
            double amount = item.value("amount", 0.0);
            std::string start = item.value("startDate", "");
            std::string end = item.value("endDate", "");
            if (end.empty()) end = "Forever";

            std::string label = "[EXP] " + name + "   $" + std::to_string((int)amount) + 
                                "   Start: " + start + "   End: " + end;

            ri.displayText.setFont(getContext().fontHolder->get(Fonts::ID::Dosis));
            ri.displayText.setString(label);
            ri.displayText.setCharacterSize(20);
            ri.displayText.setFillColor(sf::Color::White);
            ri.displayText.setPosition(110, yOffset + 5);
            
            // [FIX] Strong Outline for List Text
            ri.displayText.setOutlineColor(sf::Color::Black);
            ri.displayText.setOutlineThickness(2.0f);

            ri.background.setSize(sf::Vector2f(800, 40));
            // [FIX] Darker Background (200 alpha)
            ri.background.setFillColor(sf::Color(200, 50, 50, 200)); 
            ri.background.setPosition(100, yOffset);
            
            ri.bounds = ri.background.getGlobalBounds();

            mItems.push_back(ri);
            yOffset += 50;
        }
    }

    // 2. Load Incomes (Type 1)
    std::ifstream incFile("data/IncomeRecurring.json");
    if (incFile && incFile.peek() != std::ifstream::traits_type::eof()) {
        json root;
        try { incFile >> root; } catch (...) {}

        int index = 0;
        for (const auto& item : root) {
            RecurringItem ri;
            ri.id = index++;
            ri.type = 1;

            std::string name = item.value("sourceName", "Unknown");
            double amount = item.value("amount", 0.0);
            std::string start = item.value("startDate", "");
            std::string end = item.value("endDate", "");
            if (end.empty()) end = "Forever";

            std::string label = "[INC] " + name + "   $" + std::to_string((int)amount) + 
                                "   Start: " + start + "   End: " + end;

            ri.displayText.setFont(getContext().fontHolder->get(Fonts::ID::Dosis));
            ri.displayText.setString(label);
            ri.displayText.setCharacterSize(20);
            ri.displayText.setFillColor(sf::Color::White);
            ri.displayText.setPosition(110, yOffset + 5);

            // [FIX] Strong Outline
            ri.displayText.setOutlineColor(sf::Color::Black);
            ri.displayText.setOutlineThickness(2.0f);

            ri.background.setSize(sf::Vector2f(800, 40));
            // [FIX] Darker Background
            ri.background.setFillColor(sf::Color(50, 200, 50, 200)); 
            ri.background.setPosition(100, yOffset);

            ri.bounds = ri.background.getGlobalBounds();

            mItems.push_back(ri);
            yOffset += 50;
        }
    }
}

void RecurringState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackground);
    window.draw(mTitle);
    window.draw(mAddButtonBg);
    window.draw(mAddButtonText);
    window.draw(mBackButton);

    for (const auto& item : mItems) {
        window.draw(item.background);
        window.draw(item.displayText);
    }
}

bool RecurringState::update(sf::Time dt) { return true; }

bool RecurringState::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*getContext().window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (mBackButton.getGlobalBounds().contains(mousePosF)) {
            requestStackPop();
            return false;
        }
        if (mAddButtonBg.getGlobalBounds().contains(mousePosF)) {
            requestStackPush(States::ID::RecurringAdd);
            return false;
        }
        for (const auto& item : mItems) {
            if (item.bounds.contains(mousePosF)) {
                *getContext().activeRecordId = item.id;
                *getContext().activeRecordType = item.type;
                requestStackPush(States::ID::RecurringEdit);
                return false;
            }
        }
    }
    return false;
}