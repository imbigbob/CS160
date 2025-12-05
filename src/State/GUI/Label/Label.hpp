#ifndef LABEL_HPP
#define LABEL_HPP

#include "../../../ResourceHolder/ResourceHolder.hpp"
#include "../Component/Component.hpp"

namespace GUI {

class Label : public Component {
   public:
    typedef std::shared_ptr<Label> Ptr;

    Label(const std::string& text, const FontHolder& fonts);

    bool isSelectable() const override;

    void setText(const std::string& text);
    void setTextColor(const sf::Color& color);

    void handleMouseEvent(
        const sf::Event& event, const sf::RenderWindow& window
    ) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window)
        override;

    sf::FloatRect getGlobalBounds() const override;

   private:
    sf::Text mText;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}  // namespace GUI

#endif
