#pragma once

#include <SFML/System.hpp>
#include "Constants.hpp"


template <>
struct std::hash<SocketInfo>
{
    /**
     * @brief Hash function for SocketInfo
    */
    inline size_t operator()(const SocketInfo& s) const
    {
        size_t tmp0 = std::hash<int>()(s.first.toInteger());
        size_t tmp1 = std::hash<unsigned short>()(s.second);

        tmp0 ^= tmp1 + 0x9e3779b9 + (tmp0 << 6) + (tmp0 >> 2);
        return tmp0;
    }
};


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


template <class T>
struct std::hash<sf::Rect<T>>
{
    /**
     * @brief Hash function for sf::Rect
    */
    inline size_t operator()(const sf::Rect<T>& r) const
    {
        size_t tmp0 = std::hash<sf::Vector2f>()(r.getPosition());
        size_t tmp1 = std::hash<sf::Vector2f>()(r.getSize());

        tmp0 ^= tmp1 + 0x9e3779b9 + (tmp0 << 6) + (tmp0 >> 2);
        return tmp0;
    }
};


template <>
struct std::hash<sf::Sprite>
{
    /**
     * @brief Hash function for sf::Sprite
    */
    inline size_t operator()(const sf::Sprite& s) const
    {
        return std::hash<sf::FloatRect>()(s.getGlobalBounds());
    }
};