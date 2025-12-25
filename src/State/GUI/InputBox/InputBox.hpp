#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include "../../../ResourceHolder/ResourceHolder.hpp"
#include "../Component/Component.hpp"
#include <string>

namespace GUI
{

    class InputBox : public Component
    {
    public:
        typedef std::shared_ptr<InputBox> Ptr;

        InputBox(const FontHolder &fonts);

        bool isSelectable() const override;
        void select() override;
        void deselect() override;

        void activate() override;
        void deactivate() override;

        void setText(const std::string &text);
        std::string getText() const;
        void setMaxLength(unsigned int length);

        void handleMouseEvent(
            const sf::Event &event, const sf::RenderWindow &window) override;

        void handleEvent(const sf::Event &event, const sf::RenderWindow &window)
            override;

        sf::FloatRect getGlobalBounds() const override;

    private:
        sf::RectangleShape mBox;
        sf::Text mText;
        std::string mString;
        unsigned int mMaxLength;
        bool mHasFocus;

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

} // namespace GUI

#endif