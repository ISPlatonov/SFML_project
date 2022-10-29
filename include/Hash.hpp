#pragma once

#include <SFML/System.hpp>


template <class T>
struct std::hash<sf::Vector2<T>>
{
    /**
     * @brief Hash function for sf::Vector2
    */
    inline size_t operator()(const sf::Vector2<T>& v) const
    {
        // Compute individual hash values for first
        // and second. Combine them using the Boost-func

        size_t tmp0 = std::hash<T>()(v.x);
        size_t tmp1 = std::hash<T>()(v.y);

        tmp0 ^= tmp1 + 0x9e3779b9 + (tmp0 << 6) + (tmp0 >> 2);
        return tmp0;
    }
};