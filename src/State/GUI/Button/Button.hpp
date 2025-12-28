#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <functional>

#include "../../../ResourceHolder/ResourceHolder.hpp"
#include "../Component/Component.hpp"

namespace GUI {

class Button : public Component {
   public:
    typedef std::shared_ptr<Button> Ptr;
    typedef std::function<void()> Callback;

    Button(
        const FontHolder& fontHolder, const TextureHolder& textureHolder,
        const std::string& text, bool isDirectionButton = false,
        bool isLeft = false
    );

    void setCallback(const Callback& callback);
    void setText(const std::string& text);
    void setToggle(bool flag);
    void setSelectable(bool flag);

    bool isSelectable() const override;
    void select() override;
    void deselect() override;

    void activate() override;
    void deactivate() override;

    void handleMouseEvent(
        const sf::Event& event, const sf::RenderWindow& window
    ) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window)
        override;

    sf::FloatRect getGlobalBounds() const override;

   private:
    const sf::Texture& mNormalTexture;
    const sf::Texture& mSelectedTexture;
    const sf::Texture& mPressedTexture;

    Callback mCallback;
    sf::Sprite mSprite;
    sf::Text mText;
    bool mIsToggle = false, mIsSelectable = true;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}  // namespace GUI

#endif
