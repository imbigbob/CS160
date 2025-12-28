#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics.hpp>

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text, sf::Vector2f offset = sf::Vector2f());
std::string toString(sf::Keyboard::Key key);

#endif
