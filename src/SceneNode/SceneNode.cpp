#include "SceneNode.hpp"

#include <algorithm>
#include <cassert>

void SceneNode::attachChild(Ptr child) {
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {
    std::vector<Ptr>::iterator found =
        std::find_if(mChildren.begin(), mChildren.end(), [&](Ptr& child) {
            return child.get() == &node;
        });
    assert(found != mChildren.end());

    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

void SceneNode::handleEvent(const sf::Event& event) {
    handleEventCurrent(event);
    handleEventChildren(event);
}

void SceneNode::update(sf::Time deltaTime) {
    updateCurrent(deltaTime);
    updateChildren(deltaTime);
}

SceneNode* SceneNode::getParent() const { return mParent; }

Textures::ID SceneNode::getTextureID() const { return Textures::ID::None; }

sf::Vector2f SceneNode::getWorldPosition() const {
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const {
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node; node = node->mParent) {
        transform *= node->getTransform();
    }

    return transform;
}

void SceneNode::handleEventCurrent(const sf::Event&) {}

void SceneNode::handleEventChildren(const sf::Event& event) {
    for (Ptr& child : mChildren) {
        child->handleEvent(event);
    }
}

void SceneNode::updateCurrent(sf::Time deltaTime) {}

void SceneNode::updateChildren(sf::Time deltaTime) {
    for (Ptr& child : mChildren) {
        child->update(deltaTime);
    }
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    drawCurrent(target, states);
    drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const {}

void SceneNode::drawChildren(
    sf::RenderTarget& target, sf::RenderStates states
) const {
    for (const Ptr& child : mChildren) {
        child->draw(target, states);
    }
}
