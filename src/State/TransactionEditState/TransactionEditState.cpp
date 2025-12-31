#include "TransactionEditState.hpp"
#include "../WarningState/WarningState.hpp"
#include "model/Transaction/Transaction.hpp"
int TransactionEditState::sPendingMode = 0;
TransactionManager *TransactionEditState::sPendingTransactionManager = nullptr;
int TransactionEditState::sPendingIndex = -1;

void TransactionEditState::setPayload(int index, int mode, TransactionManager *tManager)
{
    sPendingIndex = index;
    sPendingMode = mode;
    sPendingTransactionManager = tManager;
}

TransactionEditState::TransactionEditState(StateStack &stack, Context context)
    : State(stack, context)
{
    mEditingMode = sPendingMode;
    mIndex = sPendingIndex;
    mTransactionManager = sPendingTransactionManager;

    sf::Vector2f center(
        context.window->getSize().x / 2.f,
        context.window->getSize().y / 2.f);

    const float startY = center.y - 180.f;
    const float gapY = 45.f;
    const float startX = center.x - 300.f;

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

    // Prefill
    if (mIndex >= 0)
    {
        auto &t = *mTransactionManager->getValue(mIndex);
        mAmountBox->setText(std::to_string(t.getAmount()));
        mWalletBox->setText(t.getWalletId());
        mDescBox->setText(t.getDescription());
        mTypeBox->setText(t.getTypeId());
    }

    auto saveBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Save");
    saveBtn->setPosition(center.x - 210.f, startY + gapY * 7 + 50.f);
    saveBtn->setCallback([this]()
                         { save(); });

    auto cancelBtn = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Cancel");
    cancelBtn->setPosition(center.x + 210.f, startY + gapY * 7 + 50.f);
    cancelBtn->setCallback([this]()
                           { requestStackPop(); });

    mGUIContainer.addComponent(saveBtn);
    mGUIContainer.addComponent(cancelBtn);
}

void TransactionEditState::draw()
{
    auto &window = *getContext().window;

    window.setView(window.getDefaultView());

    sf::RectangleShape dim(window.getView().getSize());
    dim.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(dim);

    window.draw(mGUIContainer);
}

bool TransactionEditState::update(sf::Time deltaTime)
{
    return true;
}

bool TransactionEditState::handleEvent(const sf::Event &event)
{
    // Delegate event handling to the container
    // This handles typing in InputBox and clicking Buttons
    mGUIContainer.handleEvent(event, *getContext().window);

    // Optional: Allow 'Enter' key to save
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        save();
    }

    return false;
}
void TransactionEditState::save()
{
    if (mIndex < 0)
        return;

    // ---- VALIDATION ----
    if (mAmountBox->getText().empty() ||
        mWalletBox->getText().empty() ||
        mTypeBox->getText().empty() ||
        mDescBox->getText().empty())
    {
        WarningState::setMessage("Please fill in all required fields!");
        requestStackPush(States::ID::Warning);
        return;
    }

    if (std::stof(mAmountBox->getText()) <= 0.f)
    {
        WarningState::setMessage("Amount must be positive!");
        requestStackPush(States::ID::Warning);
        return;
    }

    std::string walletId = mWalletBox->getText();
    if (!walletTypeManager.isTypeIdExist(walletId))
    {
        WarningState::setMessage("Wallet ID does not exist!");
        requestStackPush(States::ID::Warning);
        return;
    }
    std::string typeId = mTypeBox->getText();
    if (!typeManager.isTypeIdExist(typeId))
    {
        WarningState::setMessage("Transaction Type ID does not exist!");
        requestStackPush(States::ID::Warning);
        return;
    }
    // Transaction(        std::string typeId, double amount,        std::string walletId, std::string description)
    Transaction temp(mTypeBox->getText(), std::stod(mAmountBox->getText()), mWalletBox->getText(), mDescBox->getText());
    mTransactionManager->editTransaction(mIndex, temp);
    requestStackPop();
}
