#include "InputBox.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace GUI
{

    InputBox::InputBox(const FontHolder &fonts)
        : mBox(),
          mText("", fonts.get(Fonts::ID::Dosis), 16), // Adjust font ID/size as needed
          mString(""),
          mMaxLength(20),
          mHasFocus(false)
    {

        // Visual Setup
        mBox.setSize(sf::Vector2f(200.f, 30.f));
        mBox.setFillColor(sf::Color(255, 255, 255, 200));
        mBox.setOutlineThickness(1.f);
        mBox.setOutlineColor(sf::Color(100, 100, 100));

        mText.setFillColor(sf::Color::Black);

        // Padding
        mText.setPosition(5.f, 5.f);
    }

    bool InputBox::isSelectable() const
    {
        return true;
    }

    void InputBox::select()
    {
        Component::select();
        mBox.setOutlineColor(sf::Color::Blue);
    }

    void InputBox::deselect()
    {
        Component::deselect();
        mBox.setOutlineColor(sf::Color(100, 100, 100));
        mHasFocus = false;
    }

    void InputBox::activate()
    {
        if (isSelectable())
        {
            Component::activate();
            select();
            mHasFocus = true;
        }
    }

    void InputBox::deactivate()
    {
        Component::deactivate(); // 🔴 IMPORTANT
        deselect();
        mHasFocus = false;
    }

    void InputBox::setText(const std::string &text)
    {
        mString = text;
        mText.setString(mString);
    }

    std::string InputBox::getText() const
    {
        return mString;
    }

    void InputBox::setMaxLength(unsigned int length)
    {
        mMaxLength = length;
    }

    void InputBox::handleMouseEvent(const sf::Event &event, const sf::RenderWindow &window)
    {
        // Handle clicking to focus
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

            if (getGlobalBounds().contains(worldPos))
            {
                activate();
            }
            else
            {
                deactivate();
            }
        }
    }

    void InputBox::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
    {
        // --- 1. Handle Mouse Click (Setting Focus) ---
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

            if (getGlobalBounds().contains(worldPos))
            {
                activate(); // Sets mHasFocus = true
            }
            else
            {
                deactivate(); // Sets mHasFocus = false
            }
        }

        // --- 2. Handle Text Entry (Typing) ---
        // Only type if we have focus AND the event is TextEntered
        if (mHasFocus && event.type == sf::Event::TextEntered)
        {
            // Handle Backspace (Unicode 8)
            if (event.text.unicode == 8)
            {
                if (!mString.empty())
                    mString.pop_back();
            }
            // Handle standard ASCII characters (32-127)
            else if (event.text.unicode >= 32 && event.text.unicode < 128)
            {
                if (mString.length() < mMaxLength)
                {
                    mString += static_cast<char>(event.text.unicode);
                }
            }

            // Update the visual text
            mText.setString(mString);
        }
    }

    sf::FloatRect InputBox::getGlobalBounds() const
    {
        return getTransform().transformRect(mBox.getGlobalBounds());
    }

    void InputBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(mBox, states);
        target.draw(mText, states);
    }

} // namespace GUI