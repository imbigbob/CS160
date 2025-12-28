#include "Label.hpp"

#include "../../../Utility/Utility.hpp"

namespace GUI {

Label::Label(const std::string& text, const FontHolder& fonts)
    : mText(text, fonts.get(Fonts::ID::VTV323), 20) {}

bool Label::isSelectable() const { return false; }

sf::FloatRect Label::getGlobalBounds() const {
    return getTransform().transformRect(mText.getGlobalBounds());
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mText, states);
}

void Label::setText(const std::string& text) { mText.setString(text); }

void Label::setTextColor(const sf::Color& color) { mText.setFillColor(color); }

void Label::handleMouseEvent(
    const sf::Event& event, const sf::RenderWindow& window
) {}

void Label::handleEvent(
    const sf::Event& event, const sf::RenderWindow& window
) {}

}  // namespace GUI
