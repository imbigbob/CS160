#ifndef SPRITE_NODE_HPP
#define SPRITE_NODE_HPP

#include "../ResourceHolder/ResourceHolder.hpp"
#include "../SceneNode/SceneNode.hpp"

class SpriteNode : public SceneNode {
   public:
    SpriteNode(
        TextureHolder& textureHolder, Textures::ID textureID,
        sf::IntRect textureRect = sf::IntRect()
    );

    void setSprite(
        Textures::ID textureID, sf::IntRect textureRect = sf::IntRect()
    );
    void setTextureRect(sf::IntRect textureRect);

    sf::Vector2f getSize() const;
    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

    void setOpacity(float opacity);

   protected:
    TextureHolder& mTextureHolder;
    sf::Sprite mSprite;

    void flipHorizontally();

    void drawCurrent(
        sf::RenderTarget& target, sf::RenderStates states
    ) const override;
};

#endif
