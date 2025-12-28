#include "TextNode.hpp"

#include "../Utility/Utility.hpp"

TextNode::TextNode(
    const FontHolder& fontHolder, Fonts::ID fontID, const std::string& text,
    unsigned int size, sf::Color color
)
    : mText(text, fontHolder.get(fontID), size) {
    mText.setFillColor(color);
}

float TextNode::getTextSize() const { return mText.getLocalBounds().width; }

void TextNode::setText(const std::string& text) { mText.setString(text); }

void TextNode::setOpacity(float opacity) {
    mText.setFillColor(
        sf::Color(
            mText.getFillColor().r, mText.getFillColor().g,
            mText.getFillColor().b, 255 * opacity
        )
    );
}

void TextNode::drawCurrent(
    sf::RenderTarget& target, sf::RenderStates states
) const {
    target.draw(mText, states);
}
