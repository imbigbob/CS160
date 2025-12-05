template <typename T>
std::ofstream& operator<<(std::ofstream& fout, const sf::Vector2<T>& vector) {
    fout << vector.x << ' ' << vector.y;
    return fout;
}

template <typename T>
std::ofstream& operator<<(std::ofstream& fout, const sf::Rect<T>& rect) {
    fout << rect.left << ' ' << rect.top << ' ' << rect.width << ' '
         << rect.height;
    return fout;
}

template <typename T>
std::ifstream& operator>>(std::ifstream& fin, sf::Vector2<T>& vector) {
    fin >> vector.x >> vector.y;
    return fin;
}

template <typename T>
std::ifstream& operator>>(std::ifstream& fin, sf::Rect<T>& rect) {
    fin >> rect.left >> rect.top >> rect.width >> rect.height;
    return fin;
}
