#ifndef SETTINGS_STATE_HPP
#define SETTINGS_STATE_HPP

#include "../../Utility/Utility.hpp"
#include "../GUI/Button/Button.hpp"
#include "../GUI/Container/Container.hpp"
#include "../GUI/Label/Label.hpp"
#include "../State.hpp"

class SettingsState : public State {
   public:
    SettingsState(StateStack& stack, Context context);

    bool handleEvent(const sf::Event& event) override;
    bool update(sf::Time deltaTime) override;
    void draw() override;

   private:
    sf::Sprite mBackgroundSprite;
    sf::Sprite mFrameSprite;
    std::array<sf::Text, 2> mBindingPlayerTexts;
    GUI::Container mGUIContainer;

    std::array<
        GUI::Button::Ptr,
        2 * (static_cast<int>(Directions::ID::DirectionCount))>
        mBindingButtons;
    std::array<
        GUI::Label::Ptr, 2 * (static_cast<int>(Directions::ID::DirectionCount))>
        mBindingLabels;

    void updateLabels();
    void addDirectionButtonLabel(
        Directions::ID direction, int player, sf::Vector2f position,
        const std::string& text, Context context
    );
    void addPowerButtonLabel(
        int player, sf::Vector2f position, const std::string& text,
        Context context
    );
};

#endif
