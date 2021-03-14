#pragma once

#include <cmath>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <random>

typedef float fpt;

/**
 *
 * @tparam Iter
 * @tparam RandomGenerator
 * @param start
 * @param end
 * @param g
 * @return
 */
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

/**
 *
 * @tparam Iter
 * @param start
 * @param end
 * @return
 */
template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}

inline std::ostream &operator<<(std::ostream &os, const sf::SoundBuffer &soundBuffer) {
    os << "OneChannelSoundBuffer {Channel Count=" << soundBuffer.getChannelCount() << ", Duration(s)="
       << soundBuffer.getDuration().asSeconds() << ", Sample Count=" << soundBuffer.getSampleCount() << ", Sample Rate="
       << soundBuffer.getSampleRate() << "}";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const sf::Vector2<fpt> &vector) {
    os << "[x = " << vector.x << ", y = " << vector.y << "]";
    return os;
}

inline sf::Vector2<fpt> operator+(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x += scalar;
    ret.y += scalar;
    return ret;
}

inline sf::Vector2<fpt> operator-(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x -= scalar;
    ret.y -= scalar;
    return ret;
}

inline sf::Vector2<fpt> operator/(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x /= scalar;
    ret.y /= scalar;
    return ret;
}

inline sf::Vector2<fpt> operator*(const sf::Vector2<fpt> &lhs, fpt scalar) {
    sf::Vector2<fpt> ret(lhs);
    ret.x *= scalar;
    ret.y *= scalar;
    return ret;
}
