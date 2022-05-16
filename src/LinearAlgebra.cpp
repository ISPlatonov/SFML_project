#include "LinearAlgebra.hpp"


void linalg::normalize(sf::Vector2f& v)
{
    float m = magnitude(v);
    if (m == 0.f)
        return;
    m = std::sqrt (1.f / m);
    v.x *= m;
    v.y *= m;
}


float linalg::magnitude(const sf::Vector2f& v)
{
    auto x = static_cast<float>(v.x);
    auto y = static_cast<float>(v.y);
    return std::sqrt(x * x + y * y);
}
