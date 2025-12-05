#include "MovableSpriteNode.hpp"

#include "../Global/Global.hpp"

sf::Vector2f MovableSpriteNode::getVelocity() const { return mVelocity; }

void MovableSpriteNode::setVelocity(const sf::Vector2f& velocity) {
    mVelocity = velocity;
}

void MovableSpriteNode::updateCurrent(sf::Time deltaTime) {
    move(
        mVelocity * Global::SPEED_MODIFIER * Global::DIFFICULTY_MODIFIER *
        deltaTime.asSeconds()
    );
}

void MovableSpriteNode::saveCurrent(std::ofstream& fout) const {
    SpriteNode::saveCurrent(fout);
    fout << mVelocity << '\n';
}

void MovableSpriteNode::loadCurrent(std::ifstream& fin) {
    SpriteNode::loadCurrent(fin);
    fin >> mVelocity;
}
