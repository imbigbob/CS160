#include "TextBox.hpp"

namespace GUI {

TextBox::TextBox(const FontHolder& fonts, unsigned int charSize) {
    mText.setFont(fonts.get(Fonts::ID::Dosis));
    mText.setCharacterSize(charSize);
    mText.setFillColor(sf::Color::White);

    mBox.setFillColor(sf::Color(30, 30, 30));
    mBox.setOutlineColor(sf::Color::White);
    mBox.setOutlineThickness(2);
}

void TextBox::setPosition(float x, float y) {
    mBox.setPosition(x, y);
    mText.setPosition(x + 8.f, y + 4.f);
}

void TextBox::setSize(float width, float height) {
    mBox.setSize(sf::Vector2f(width, height));
}

void TextBox::setText(const std::string& str) { mText.setString(str); }

std::string TextBox::getString() const { return mText.getString(); }

void TextBox::setTextColor(const sf::Color& color) {
    mText.setFillColor(color);
}

void TextBox::setMaxLength(int max) { mMaxLength = max; }

sf::FloatRect TextBox::getGlobalBounds() const {
    return mBox.getGlobalBounds();
}

// -------------------
// MOUSE CLICK FOCUS
// -------------------
void TextBox::handleMouseEvent(
    const sf::Event& event, const sf::RenderWindow& window
) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

        mFocused = mBox.getGlobalBounds().contains(mouse);

        mBox.setOutlineColor(mFocused ? sf::Color::Green : sf::Color::White);
    }
}

// -------------------
// KEYBOARD INPUT
// -------------------
void TextBox::handleEvent(
    const sf::Event& event, const sf::RenderWindow& window
) {
    if (!mFocused) return;

    // Typing
    if (event.type == sf::Event::TextEntered) {
        char c = static_cast<char>(event.text.unicode);

        // Backspace
        if (event.text.unicode == 8) {
            auto txt = mText.getString();
            if (!txt.isEmpty()) {
                txt.erase(txt.getSize() - 1);
                mText.setString(txt);
            }
            return;
        }

        // Printable ASCII
        if (c >= 32 && c <= 126) {
            if (mMaxLength < 0 ||
                (int)mText.getString().getSize() < mMaxLength) {
                mText.setString(mText.getString() + c);
            }
        }
    }
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mBox, states);
    target.draw(mText, states);

    // caret blinking
    if (mFocused) {
        if (mCaretClock.getElapsedTime().asSeconds() > 0.5f) {
            const_cast<TextBox*>(this)->mShowCaret = !mShowCaret;
            const_cast<TextBox*>(this)->mCaretClock.restart();
        }

        if (mShowCaret) {
            sf::RectangleShape caret;
            caret.setFillColor(sf::Color::White);

            float x = mText.getGlobalBounds().left +
                      mText.getGlobalBounds().width + 2.f;
            float y = mText.getPosition().y;

            caret.setSize(sf::Vector2f(2.f, mText.getCharacterSize()));
            caret.setPosition(x, y);

            target.draw(caret, states);
        }
    }
}

}  // namespace GUI
