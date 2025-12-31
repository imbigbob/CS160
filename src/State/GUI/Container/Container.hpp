#ifndef GUI_CONTAINER_HPP
#define GUI_CONTAINER_HPP

#include "../../../core/DynamicArray/DynamicArray.hpp"

#include "../Component/Component.hpp"

namespace GUI
{

    class Container : public Component
    {
    public:
        typedef std::shared_ptr<Container> Ptr;

        Container(bool isDirectionButton = false);

        void addComponent(Component::Ptr component);

        bool isSelectable() const override;
        void handleMouseEvent(
            const sf::Event &event, const sf::RenderWindow &window) override;
        void handleEvent(const sf::Event &event, const sf::RenderWindow &window)
            override;

    private:
        DynamicArray<Component::Ptr> mChildren;
        int mSelectedChild;
        bool isDirectionButton;

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        bool hasSelection() const;
        void select(int index);
        void selectNext();
        void selectPrevious();
    };

} // namespace GUI

#endif
