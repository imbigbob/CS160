#include "Tile.hpp"

#include <cmath>

#include "../Global/Global.hpp"

Tile::Tile(Type type, sf::Vector2f position, Directions::ID direction)
    : mType(type), mDirection(direction) {
    setPosition(position);
}

Tile::Type Tile::getType() const { return mType; }

Directions::ID Tile::getDirection() const { return mDirection; }

sf::Vector2f Tile::getVelocity() const { return mVelocity; }

void Tile::setVelocity(const sf::Vector2f& velocity) { mVelocity = velocity; }

float Tile::distanceTo(Tile* other) const {
    sf::Vector2f position = getWorldPosition(),
                 otherPosition = other->getWorldPosition();
    return std::hypotf(
        position.x - otherPosition.x, position.y - otherPosition.y
    );
}

void Tile::updateCurrent(sf::Time deltaTime) {
    switch (mDirection) {
        case Directions::ID::Left:
            move(
                -mVelocity * Global::SPEED_MODIFIER *
                Global::DIFFICULTY_MODIFIER * deltaTime.asSeconds()
            );
            break;

        case Directions::ID::Right:
            move(
                mVelocity * Global::SPEED_MODIFIER *
                Global::DIFFICULTY_MODIFIER * deltaTime.asSeconds()
            );
            break;

        default:
            break;
    }
}

void Tile::saveCurrent(std::ofstream& fout) const {
    SceneNode::saveCurrent(fout);
    fout << mType << ' ' << static_cast<int>(mDirection) << '\n';
    fout << mVelocity << '\n';
}

void Tile::loadCurrent(std::ifstream& fin) {
    SceneNode::loadCurrent(fin);
    int type, direction;
    fin >> type;
    mType = static_cast<Type>(type);

    fin >> direction;
    mDirection = static_cast<Directions::ID>(direction);

    fin >> mVelocity;
}
