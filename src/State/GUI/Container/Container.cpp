#include "Container.hpp"

namespace GUI {

Container::Container(bool isDirectionButton)
    : mSelectedChild(-1), isDirectionButton(isDirectionButton) {}

void Container::addComponent(Component::Ptr component) {
    mChildren.push_back(component);

    if (!hasSelection() && component->isSelectable()) {
        select(mChildren.size() - 1);
    }
}

bool Container::isSelectable() const { return false; }

void Container::handleMouseEvent(
    const sf::Event& event, const sf::RenderWindow& window
) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        for (int i = 0; i < mChildren.size(); i++) {
            if (!mChildren[i]->isSelectable()) {
                continue;
            }

            sf::FloatRect bounds = mChildren[i]->getGlobalBounds();

            if (bounds.contains(mousePosition.x, mousePosition.y)) {
                select(i);
            }
        }
    } else if (event.type == sf::Event::MouseButtonPressed) {
        if (!hasSelection()) {
            return;
        }

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        for (int i = 0; i < mChildren.size(); i++) {
            sf::FloatRect bounds = mChildren[i]->getGlobalBounds();

            if (bounds.contains(mousePosition.x, mousePosition.y)) {
                if (!mChildren[i]->isSelectable()) {
                    continue;
                }

                mChildren[mSelectedChild]->deactivate();
                mChildren[i]->activate();
                mSelectedChild = i;
            }
        }
    }
}

void Container::handleEvent(
    const sf::Event& event, const sf::RenderWindow& window
) {
    if (hasSelection() && mChildren[mSelectedChild]->isActive()) {
        mChildren[mSelectedChild]->handleEvent(event, window);
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::W:
            case sf::Keyboard::Up:
                selectPrevious();
                break;

            case sf::Keyboard::S:
            case sf::Keyboard::Down:
                selectNext();
                break;

            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                if (hasSelection() && !isDirectionButton) {
                    mChildren[mSelectedChild]->activate();
                }

                break;
        }
    } else {
        handleMouseEvent(event, window);
    }
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const Component::Ptr& child : mChildren) {
        target.draw(*child, states);
    }
}

bool Container::hasSelection() const { return mSelectedChild >= 0; }

void Container::select(int index) {
    if (mChildren[index]->isSelectable()) {
        if (hasSelection()) {
            mChildren[mSelectedChild]->deselect();
        }

        mChildren[index]->select();
        mSelectedChild = index;
    }
}

void Container::selectNext() {
    if (!hasSelection()) {
        return;
    }

    int next = mSelectedChild;

    do {
        next = (next + 1) % mChildren.size();
    } while (!mChildren[next]->isSelectable());

    select(next);
}

void Container::selectPrevious() {
    if (!hasSelection()) {
        return;
    }

    int prev = mSelectedChild;

    do {
        prev = (prev + mChildren.size() - 1) % mChildren.size();
    } while (!mChildren[prev]->isSelectable());

    select(prev);
}

}  // namespace GUI
