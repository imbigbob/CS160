#include "MasterManagementEditState.hpp"
#include "../GUI/Button/Button.hpp"
#include "../../core/IncomeManagement/IncomeManagement.hpp"
#include "../../core/ExpenseManagement/ExpenseManagement.hpp"
#include "../../core/WalletManager/WalletManager.hpp"

MasterManagementEditState::MasterManagementEditState(StateStack& stack, Context context)
    : State(stack, context)
{
    sf::Vector2f windowSize(context.window->getSize());
    mType = *getContext().activeRecordType;
    mTargetId = *getContext().activeRecordId;

    mBackgroundSprite.setTexture(context.textureHolder->get(Textures::ID::MenuBackground));
    mBackgroundSprite.setScale(windowSize.x / mBackgroundSprite.getLocalBounds().width, windowSize.y / mBackgroundSprite.getLocalBounds().height);

    // Fetch existing name
    if (mType == 0) mCurrentInput = context.expenseManagement->getName(mTargetId);
    else if (mType == 1) { /* Assume IncomeManager has getName() */ 
         // If IncomeManager doesn't have getName(), you must add it or iterate to find it
         // Implementation for iteration:
         auto& list = context.incomeManagement->getAll();
         for(auto& i : list) if(i.id == mTargetId) mCurrentInput = i.name;
    }
    else if (mType == 2) mCurrentInput = context.walletManager->getWalletName(mTargetId);

    mTitleText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mTitleText.setString("Edit Item");
    mTitleText.setCharacterSize(40);
    mTitleText.setPosition(windowSize.x / 2.f - 80.f, 100.f);

    mInputBox.setSize(sf::Vector2f(400.f, 50.f));
    mInputBox.setFillColor(sf::Color(255, 255, 255, 200));
    mInputBox.setPosition(windowSize.x / 2.f - 200.f, 200.f);
    mInputBox.setOutlineThickness(2.f);
    mInputBox.setOutlineColor(sf::Color::White);

    mInputText.setFont(context.fontHolder->get(Fonts::ID::Dosis));
    mInputText.setString(mCurrentInput + "|");
    mInputText.setCharacterSize(30);
    mInputText.setFillColor(sf::Color::Black);
    mInputText.setPosition(mInputBox.getPosition().x + 10.f, mInputBox.getPosition().y + 5.f);

    auto saveBtn = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Update");
    saveBtn->setPosition(windowSize.x / 2.f - 100.f, 300.f);
    saveBtn->setCallback([this]() {
        if (mCurrentInput.empty()) return;

        if (mType == 0) getContext().expenseManagement->editExpenseType(mTargetId, mCurrentInput);
        else if (mType == 1) getContext().incomeManagement->editIncomeType(mTargetId, mCurrentInput);
        else if (mType == 2) getContext().walletManager->editWallet(mTargetId, mCurrentInput);

        requestStackPop();
    });

    auto cancelBtn = std::make_shared<GUI::Button>(*context.fontHolder, *context.textureHolder, "Cancel");
    cancelBtn->setPosition(windowSize.x / 2.f + 50.f, 300.f);
    cancelBtn->setCallback([this]() { requestStackPop(); });

    mGUIContainer.addComponent(saveBtn);
    mGUIContainer.addComponent(cancelBtn);
}

bool MasterManagementEditState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);

    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) {
            if (!mCurrentInput.empty()) mCurrentInput.pop_back();
        }
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            mCurrentInput += static_cast<char>(event.text.unicode);
        }
        mInputText.setString(mCurrentInput + "|");
    }
    return false;
}

bool MasterManagementEditState::update(sf::Time deltaTime) { return true; }

void MasterManagementEditState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    window.draw(mTitleText);
    window.draw(mInputBox);
    window.draw(mInputText);
    window.draw(mGUIContainer);
}