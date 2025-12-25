#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <memory>

namespace GUI
{

    class Component : public sf::Drawable,
                      public sf::Transformable,
                      private sf::NonCopyable
    {
    public:
        typedef std::shared_ptr<Component> Ptr;

        Component();
        virtual ~Component();

        virtual bool isSelectable() const = 0;
        bool isSelected() const;
        virtual void select();
        virtual void deselect();

        virtual bool isActive() const;
        virtual void activate();
        virtual void deactivate();

        virtual void handleMouseEvent(
            const sf::Event &event, const sf::RenderWindow &window) = 0;
        virtual void handleEvent(
            const sf::Event &event, const sf::RenderWindow &window) = 0;

        virtual sf::FloatRect getGlobalBounds() const;

    private:
        bool mIsSelected;
        bool mIsActive;
    };

} // namespace GUI

#endif
