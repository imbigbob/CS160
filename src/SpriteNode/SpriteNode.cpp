#include "SpriteNode.hpp"

SpriteNode::SpriteNode(
    TextureHolder& textureHolder, Textures::ID textureID,
    sf::IntRect textureRect
)
    : mTextureHolder(textureHolder) {
    setSprite(textureID, textureRect);
}

void SpriteNode::setSprite(Textures::ID textureID, sf::IntRect textureRect) {
    mSprite.setTexture(mTextureHolder.get(textureID));

    if (textureRect != sf::IntRect()) {
        mSprite.setTextureRect(textureRect);
    }
}

void SpriteNode::setTextureRect(sf::IntRect textureRect) {
    mSprite.setTextureRect(textureRect);
}

sf::Vector2f SpriteNode::getSize() const {
    return mSprite.getLocalBounds().getSize();
}

sf::FloatRect SpriteNode::getLocalBounds() const {
    return mSprite.getLocalBounds();
}

sf::FloatRect SpriteNode::getGlobalBounds() const {
    return getWorldTransform().transformRect(getLocalBounds());
}

void SpriteNode::setOpacity(float opacity) {
    mSprite.setColor(sf::Color(255, 255, 255, 255 * opacity));
}

void SpriteNode::flipHorizontally() {
    sf::IntRect textureRect = mSprite.getTextureRect();
    textureRect.left += textureRect.width;
    textureRect.width = -textureRect.width;
    mSprite.setTextureRect(textureRect);
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states)
    const {
    target.draw(mSprite, states);
}

void SpriteNode::saveCurrent(std::ofstream& fout) const {
    SceneNode::saveCurrent(fout);
    fout << mSprite.getTextureRect() << '\n';
}

void SpriteNode::loadCurrent(std::ifstream& fin) {
    SceneNode::loadCurrent(fin);
    sf::IntRect textureRect;
    fin >> textureRect;
    setTextureRect(textureRect);
}
