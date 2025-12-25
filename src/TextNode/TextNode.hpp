#ifndef TEXT_NODE_HPP
#define TEXT_NODE_HPP

#include "../ResourceHolder/ResourceHolder.hpp"
#include "../SceneNode/SceneNode.hpp"

class TextNode : public SceneNode {
   public:
    typedef std::unique_ptr<TextNode> Ptr;

    TextNode(
        const FontHolder& fontHolder, Fonts::ID fontID, const std::string& text,
        unsigned int size, sf::Color color = sf::Color::Black
    );

    float getTextSize() const;

    void setText(const std::string& text);
    void setOpacity(float opacity);

   private:
    sf::Text mText;

    void drawCurrent(
        sf::RenderTarget& target, sf::RenderStates states
    ) const override;
};

#endif
