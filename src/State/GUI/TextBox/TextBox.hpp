#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "../../../ResourceHolder/ResourceHolder.hpp"
#include "../Component/Component.hpp"

namespace GUI {

class TextBox : public Component {
   public:
    using Ptr = std::shared_ptr<TextBox>;

    TextBox(const FontHolder& fonts, unsigned int charSize = 24);

    bool isSelectable() const override { return true; }

    void setPosition(float x, float y);
    void setSize(float width, float height);

    void setText(const std::string& text);
    std::string getString() const;

    void setTextColor(const sf::Color& color);
    void setMaxLength(int max);

    // event handlers
    void handleMouseEvent(
        const sf::Event& event, const sf::RenderWindow& window
    ) override;

    void handleEvent(
        const sf::Event& event, const sf::RenderWindow& window
    ) override;

    sf::FloatRect getGlobalBounds() const override;

   private:
    sf::RectangleShape mBox;
    sf::Text mText;

    bool mFocused = false;
    int mMaxLength = -1;

    // caret blink
    bool mShowCaret = false;
    sf::Clock mCaretClock;

    virtual void draw(
        sf::RenderTarget& target, sf::RenderStates states
    ) const override;
};

}  // namespace GUI

#endif
