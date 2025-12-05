#include "SettingsState.hpp"

SettingsState::SettingsState(StateStack& stack, Context context)
    : State(stack, context) {
    // sf::Vector2f windowSize(context.window->getSize());
    // sf::Texture& backgroundTexture =
    //     context.textureHolder->get(Textures::ID::SettingBackground);

    // mBackgroundSprite.setTexture(backgroundTexture);
    // mBackgroundSprite.setScale(
    //     windowSize.x / backgroundTexture.getSize().x,
    //     windowSize.y / backgroundTexture.getSize().y
    // );

    // sf::Texture& frameTexture =
    // context.textureHolder->get(Textures::ID::Frame);
    // mFrameSprite.setTexture(frameTexture);
    // centerOrigin(mFrameSprite);
    // mFrameSprite.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
    // mFrameSprite.setScale(800.f / frameTexture.getSize().x, 1.f);

    // for (int i = 0; i < 2; ++i) {
    //     centerOrigin(mBindingPlayerTexts[i]);
    //     mBindingPlayerTexts[i].setString("Player " + std::to_string(i + 1));
    //     mBindingPlayerTexts[i].setFont(context.fontHolder->get(Fonts::ID::VTV323
    //     ));
    //     mBindingPlayerTexts[i].setCharacterSize(35);
    //     mBindingPlayerTexts[i].setPosition(
    //         windowSize.x / 2.f + (i == 0 ? -250.f : 150.f),
    //         windowSize.y / 2.f - 235.f
    //     );
    // }

    // for (int player = 1; player <= 2; ++player) {
    //     addDirectionButtonLabel(
    //         Directions::ID::Left, player, sf::Vector2f(-200.f, 250.f),
    //         "Move Left", context
    //     );
    //     addDirectionButtonLabel(
    //         Directions::ID::Right, player, sf::Vector2f(-200.f, 300.f),
    //         "Move Right", context
    //     );
    //     addDirectionButtonLabel(
    //         Directions::ID::Up, player, sf::Vector2f(-200.f, 350.f), "Move
    //         Up", context
    //     );
    //     addDirectionButtonLabel(
    //         Directions::ID::Down, player, sf::Vector2f(-200.f, 400.f),
    //         "Move Down", context
    //     );

    //     addPowerButtonLabel(
    //         PowerUp::Type::Immortality, player, sf::Vector2f(-200.f, 450.f),
    //         "Immortality", context
    //     );
    //     addPowerButtonLabel(
    //         PowerUp::Type::SlowTime, player, sf::Vector2f(-200.f, 500.f),
    //         "Slow Time", context
    //     );
    // }

    // updateLabels();

    // auto resetButton = std::make_shared<GUI::Button>(
    //     *context.fontHolder, *context.textureHolder, "Reset"
    // );
    // resetButton->setPosition(windowSize.x / 2, 600.f);
    // resetButton->setCallback([this]() {
    //     getContext().playerSettings1->setToDefault();
    //     getContext().powerUpSettings1->setToDefault();
    //     getContext().playerSettings2->setToDefault(true);
    //     getContext().powerUpSettings2->setToDefault(true);
    //     updateLabels();
    // });

    // auto backButton = std::make_shared<GUI::Button>(
    //     *context.fontHolder, *context.textureHolder, "Back"
    // );
    // backButton->setPosition(250.f, 50.f);
    // backButton->setCallback([this]() { requestStackPop(); });

    // mGUIContainer.addComponent(resetButton);
    // mGUIContainer.addComponent(backButton);
}

bool SettingsState::handleEvent(const sf::Event& event) {
    // bool isKeyBinding = false;
    // int directionCount = static_cast<int>(Directions::ID::DirectionCount);
    // int actionCount = directionCount + PowerUp::Type::TypeButtonCount;

    // for (size_t buttonIndex = 0; buttonIndex < mBindingButtons.size();
    //      ++buttonIndex) {
    //     if (mBindingButtons[buttonIndex]->isActive()) {
    //         isKeyBinding = true;

    //         if (buttonIndex < actionCount &&
    //             event.type == sf::Event::KeyReleased) {
    //             if (buttonIndex >= directionCount) {
    //                 getContext().powerUpSettings1->assignKey(
    //                     event.key.code,
    //                     static_cast<PowerUp::Type>(buttonIndex -
    //                     directionCount)
    //                 );
    //             } else {
    //                 getContext().playerSettings1->assignKey(
    //                     event.key.code,
    //                     static_cast<Directions::ID>(buttonIndex)
    //                 );
    //             }

    //             mBindingButtons[buttonIndex]->deactivate();
    //         } else if (event.type == sf::Event::KeyReleased) {
    //             if (buttonIndex >= directionCount + actionCount) {
    //                 getContext().powerUpSettings2->assignKey(
    //                     event.key.code,
    //                     static_cast<PowerUp::Type>(
    //                         buttonIndex - directionCount - actionCount
    //                     )
    //                 );
    //             } else {
    //                 getContext().playerSettings2->assignKey(
    //                     event.key.code,
    //                     static_cast<Directions::ID>(buttonIndex -
    //                     actionCount)
    //                 );
    //             }

    //             mBindingButtons[buttonIndex]->deactivate();
    //         }

    //         break;
    //     }
    // }

    // if (isKeyBinding) {
    //     updateLabels();
    // } else {
    //     mGUIContainer.handleEvent(event, *getContext().window);
    // }

    return false;
}

bool SettingsState::update(sf::Time deltaTime) { return true; }

void SettingsState::draw() {
    sf::RenderWindow& window = *getContext().window;

    window.draw(mBackgroundSprite);
    window.draw(mFrameSprite);
    window.draw(mBindingPlayerTexts[0]);
    window.draw(mBindingPlayerTexts[1]);
    window.draw(mGUIContainer);
}

void SettingsState::updateLabels() {
    // sf::Keyboard::Key key1;
    // sf::Keyboard::Key key2;
    // int directionCount = static_cast<int>(Directions::ID::DirectionCount);
    // int actionCount = directionCount + PowerUp::Type::TypeButtonCount;

    // for (int i = 0; i < actionCount; ++i) {
    //     if (i < directionCount) {
    //         key1 = getContext().playerSettings1->getAssignedKey(
    //             static_cast<Directions::ID>(i)
    //         );
    //         key2 = getContext().playerSettings2->getAssignedKey(
    //             static_cast<Directions::ID>(i)
    //         );
    //     } else {
    //         key1 = getContext().powerUpSettings1->getAssignedKey(
    //             static_cast<PowerUp::Type>(i - directionCount)
    //         );
    //         key2 = getContext().powerUpSettings2->getAssignedKey(
    //             static_cast<PowerUp::Type>(i - directionCount)
    //         );
    //     }

    //     mBindingLabels[i]->setText(toString(key1));
    //     mBindingLabels[i]->setTextColor(sf::Color(162, 145, 116));
    //     mBindingLabels[i + actionCount]->setText(toString(key2));
    //     mBindingLabels[i + actionCount]->setTextColor(sf::Color(162, 145,
    //     116));
    // }
}

void SettingsState::addDirectionButtonLabel(
    Directions::ID direction, int player, sf::Vector2f position,
    const std::string& text, Context context
) {
    // int index = static_cast<int>(direction);

    // if (player == 2) {
    //     position.x = -position.x;
    //     index +=
    //         (static_cast<int>(Directions::ID::DirectionCount) +
    //          PowerUp::Type::TypeButtonCount);
    // }

    // sf::Vector2f windowSize(context.window->getSize());

    // mBindingButtons[index] = std::make_shared<GUI::Button>(
    //     *context.fontHolder, *context.textureHolder, text
    // );
    // mBindingButtons[index]->setPosition(
    //     windowSize.x / 2.f + position.x, position.y
    // );
    // mBindingButtons[index]->setToggle(true);

    // mBindingLabels[index] =
    //     std::make_shared<GUI::Label>("", *context.fontHolder);
    // mBindingLabels[index]->setPosition(
    //     windowSize.x / 2.f + position.x + 120.f, position.y
    // );

    // mGUIContainer.addComponent(mBindingButtons[index]);
    // mGUIContainer.addComponent(mBindingLabels[index]);
}

void SettingsState::addPowerButtonLabel(
    int player, sf::Vector2f position, const std::string& text, Context context
) {
    // int index = static_cast<int>(Directions::ID::DirectionCount) + power;

    // if (player == 2) {
    //     position.x = -position.x;
    //     index +=
    //         (static_cast<int>(Directions::ID::DirectionCount) +
    //          PowerUp::Type::TypeButtonCount);
    // }

    // sf::Vector2f windowSize(context.window->getSize());

    // mBindingButtons[index] = std::make_shared<GUI::Button>(
    //     *context.fontHolder, *context.textureHolder, text
    // );
    // mBindingButtons[index]->setPosition(
    //     windowSize.x / 2.f + position.x, position.y
    // );
    // mBindingButtons[index]->setToggle(true);

    // mBindingLabels[index] =
    //     std::make_shared<GUI::Label>("", *context.fontHolder);
    // mBindingLabels[index]->setPosition(
    //     windowSize.x / 2.f + position.x + 120.f, position.y
    // );

    // mGUIContainer.addComponent(mBindingButtons[index]);
    // mGUIContainer.addComponent(mBindingLabels[index]);
}
