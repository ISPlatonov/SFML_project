#include "LinearAlgebra.hpp"


sf::Vector2f linalg::normalize(sf::Vector2f v)
{
    float m = magnitude(v);
    if (m == 0)
        return v;
    m = (1.f / m);
    v.x *= m;
    v.y *= m;

    return v;
}


float linalg::magnitude(const sf::Vector2f& v)
{
    auto x = static_cast<float>(v.x);
    auto y = static_cast<float>(v.y);
    return std::sqrt(x * x + y * y);
}
