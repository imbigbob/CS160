#include "FileIO.hpp"

#include <limits>

std::ofstream& operator<<(std::ofstream& fout, const sf::Text& text) {
    fout << text.getString().toAnsiString();
    return fout;
}

std::ofstream& operator<<(std::ofstream& fout, const sf::Time& time) {
    fout << time.asSeconds();
    return fout;
}

std::ofstream& operator<<(std::ofstream& fout, sf::Keyboard::Key key) {
    fout << static_cast<int>(key);
    return fout;
}

std::ifstream& operator>>(std::ifstream& fin, sf::Text& text) {
    std::string str;
    getline(fin, str);
    fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    text.setString(str);
    return fin;
}

std::ifstream& operator>>(std::ifstream& fin, sf::Time& time) {
    float seconds;
    fin >> seconds;
    time = sf::seconds(seconds);
    return fin;
}

std::ifstream& operator>>(std::ifstream& fin, sf::Keyboard::Key& key) {
    int keyInt;
    fin >> keyInt;
    key = static_cast<sf::Keyboard::Key>(keyInt);
    return fin;
}
