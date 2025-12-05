#ifndef TILE_HPP
#define TILE_HPP

#include "../SceneNode/SceneNode.hpp"

class Tile : public SceneNode {
   public:
    typedef std::unique_ptr<Tile> Ptr;

    enum Type {
        Good,
        Bad,
        None,
    };

    Tile(
        Type type, sf::Vector2f position,
        Directions::ID direction = Directions::ID::None
    );

    Type getType() const;
    Directions::ID getDirection() const;
    sf::Vector2f getVelocity() const;

    void setVelocity(const sf::Vector2f& velocity);

    float distanceTo(Tile* other) const;

   private:
    Type mType;
    Directions::ID mDirection;
    sf::Vector2f mVelocity;

    void updateCurrent(sf::Time deltaTime) override;

    void saveCurrent(std::ofstream& fout) const final;
    void loadCurrent(std::ifstream& fin) final;
};

#endif
