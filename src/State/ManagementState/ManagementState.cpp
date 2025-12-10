#include "ManagementState.hpp"

ManagementState::ManagementState(StateStack& stack, Context context)
    : State(stack, context) {
    sf::Vector2f windowSize(context.window->getSize());
    sf::Texture& backgroundTexture =
        context.textureHolder->get(Textures::ID::MenuBackground);

    mBackgroundSprite.setTexture(backgroundTexture);
    mBackgroundSprite.setScale(
        windowSize.x / backgroundTexture.getSize().x,
        windowSize.y / backgroundTexture.getSize().y
    );

    auto backButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Back"
    );
    backButton->setPosition(250.f, 50.f);
    backButton->setCallback([this]() { requestStackPop(); });
    modeSwitchButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Income"
    );
    modeSwitchButton->setPosition(450.f, 50.f);
    modeSwitchButton->setCallback([this]() {
        // rotate enum
        if (currentMode == Mode::Income)
            currentMode = Mode::Expense;
        else if (currentMode == Mode::Expense)
            currentMode = Mode::Wallet;
        else
            currentMode = Mode::Income;

        // change button text
        switch (currentMode) {
            case Mode::Income:
                modeSwitchButton->setText("Income");
                break;
            case Mode::Expense:
                modeSwitchButton->setText("Expense");
                break;
            case Mode::Wallet:
                modeSwitchButton->setText("Wallet");
                break;
        }

        loadList();
    });
    addButton = std::make_shared<GUI::Button>(
        *context.fontHolder, *context.textureHolder, "Add"
    );
    addButton->setPosition(700.f, 50.f);
    addButton->setCallback([this]() {
        // here you open create-type window
        // or push another state
    });
    mGUIContainer.addComponent(addButton);
    mGUIContainer.addComponent(modeSwitchButton);

    mGUIContainer.addComponent(backButton);
    loadList();
}

bool ManagementState::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event, *getContext().window);
    if (event.type == sf::Event::MouseWheelScrolled) {
        scrollOffset -= event.mouseWheelScroll.delta * 30.f;

        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }

    return false;
}

bool ManagementState::update(sf::Time deltaTime) { return true; }

void ManagementState::draw() {
    sf::RenderWindow& window = *getContext().window;

    window.draw(mBackgroundSprite);
    // window.draw(mFrameSprite);
    window.draw(mGUIContainer);

    // clipping view (so list stays in a box)
    sf::View oldView = window.getView();
    sf::View listView = oldView;
    listView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    listView.setCenter(oldView.getCenter());
    window.setView(listView);

    for (auto& item : listItems) {
        sf::Transform transform;
        transform.translate(0.f, -scrollOffset);
        window.draw(*item, transform);
    }

    window.setView(oldView);
}

void ManagementState::loadList() {
    listItems.clear();

    TypeManager* manager = nullptr;

    switch (currentMode) {
        case Mode::Income:
            manager = &incomeTypeManager;
            break;
        case Mode::Expense:
            manager = &expenseTypeManager;
            break;
        case Mode::Wallet:
            manager = &walletTypeManager;
            break;
    }

    DynamicArray<Type>& types = manager->getAllTypes();

    float y = listStartY;

    for (int i = 0; i < types.getSize(); i++) {
        auto label = std::make_shared<GUI::Label>(
            types[i].getName(), *getContext().fontHolder
        );
        label->setPosition(300.f, y);
        listItems.push_back(label);
        y += listItemHeight;
    }

    maxScroll = std::max(0.f, y - 500.f);  // 500px visible height
}
