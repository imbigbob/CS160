#ifndef MOVABLE_SPRITE_NODE_HPP
#define MOVABLE_SPRITE_NODE_HPP

#include "../SpriteNode/SpriteNode.hpp"

class MovableSpriteNode : public SpriteNode {
   public:
    using SpriteNode::SpriteNode;

    sf::Vector2f getVelocity() const;
    void setVelocity(const sf::Vector2f& velocity);

   protected:
    sf::Vector2f mVelocity;

    void updateCurrent(sf::Time deltaTime) override;

    void saveCurrent(std::ofstream& fout) const override;
    void loadCurrent(std::ifstream& fin) override;
};

#endif
