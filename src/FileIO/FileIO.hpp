#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <SFML/Graphics.hpp>
#include <fstream>

template <typename T>
std::ofstream& operator<<(std::ofstream& fout, const sf::Vector2<T>& vector);

template <typename T>
std::ofstream& operator<<(std::ofstream& fout, const sf::Rect<T>& rect);

std::ofstream& operator<<(std::ofstream& fout, const sf::Text& text);

std::ofstream& operator<<(std::ofstream& fout, const sf::Time& time);

std::ofstream& operator<<(std::ofstream& fout, sf::Keyboard::Key key);

template <typename T>
std::ifstream& operator>>(std::ifstream& fin, sf::Vector2<T>& vector);

template <typename T>
std::ifstream& operator>>(std::ifstream& fin, sf::Rect<T>& rect);

std::ifstream& operator>>(std::ifstream& fin, sf::Text& text);

std::ifstream& operator>>(std::ifstream& fin, sf::Time& time);

std::ifstream& operator>>(std::ifstream& fin, sf::Keyboard::Key& key);

#include "FileIO.inl"

#endif
