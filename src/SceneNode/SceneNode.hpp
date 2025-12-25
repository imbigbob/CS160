#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>
#include <set>
#include <vector>

#include "../Identifier/Identifier.hpp"
class Player;

class SceneNode : public sf::Transformable,
                  public sf::Drawable,
                  private sf::NonCopyable {
   public:
    typedef std::unique_ptr<SceneNode> Ptr;

    void attachChild(Ptr child);
    Ptr detachChild(const SceneNode& node);

    void handleEvent(const sf::Event& event);
    void update(sf::Time deltaTime);

    SceneNode* getParent() const;
    virtual Textures::ID getTextureID() const;

    sf::Vector2f getWorldPosition() const;
    sf::Transform getWorldTransform() const;

   protected:
   private:
    std::vector<Ptr> mChildren;
    SceneNode* mParent = nullptr;

    virtual void handleEventCurrent(const sf::Event& event);
    void handleEventChildren(const sf::Event& event);

    virtual void updateCurrent(sf::Time deltaTime);
    void updateChildren(sf::Time deltaTime);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void drawCurrent(
        sf::RenderTarget& target, sf::RenderStates states
    ) const;
    void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
