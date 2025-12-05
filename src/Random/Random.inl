#include <stdexcept>

template <typename T>
T Random<T>::generate(T min, T max) {
    if constexpr (std::is_floating_point_v<T>) {
        return std::uniform_real_distribution<T>(min, max)(mEngine);
    } else if constexpr (std::is_integral_v<T>) {
        return std::uniform_int_distribution<T>(min, max)(mEngine);
    }

    throw std::invalid_argument("Invalid type");
}

template <typename T>
T Random<T>::generate(const std::vector<T>& list) {
    return list[Random<size_t>::generate(0, list.size() - 1)];
}

template <typename T>
T Random<T>::generate(
    const std::vector<T>& list, const std::vector<int>& percent
) {
    if (list.size() != percent.size()) {
        throw std::invalid_argument("Invalid size, must be equal");
    }

    int sum = 0;

    for (int p : percent) {
        if (p < 0) {
            throw std::invalid_argument("Invalid percent, must be >= 0");
        }

        sum += p;
    }

    if (sum != 100) {
        throw std::invalid_argument("Invalid percent, must sum to 100");
    }

    int random = Random<int>::generate(1, 100);
    int current = 0;

    for (size_t i = 0; i < percent.size(); ++i) {
        current += percent[i];

        if (random <= current) {
            return list[i];
        }
    }

    throw std::invalid_argument("Invalid percent");
}
