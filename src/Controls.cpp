#include "Controls.hpp"


sf::Vector2f Controls::direction;
bool Controls::left = 0,
     Controls::right = 0,
     Controls::up = 0,
     Controls::down = 0;
intmax_t Controls::last_action_timepoint;


void Controls::addEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
        switch (event.key.code)
        {
            case (sf::Keyboard::A):
                left = 1;
                break;
            case (sf::Keyboard::D):
                right = 1;
                break;
            case (sf::Keyboard::W):
                up = 1;
                break;
            case (sf::Keyboard::S):
                down = 1;
                break;
            default:
                break;
        }
    else if (event.type == sf::Event::KeyReleased)
        switch (event.key.code)
        {
            case (sf::Keyboard::A):
                left = 0;
                break;
            case (sf::Keyboard::D):
                right = 0;
                break;
            case (sf::Keyboard::W):
                up = 0;
                break;
            case (sf::Keyboard::S):
                down = 0;
                break;
            default:
                break;
        }
}


sf::Vector2f Controls::getDirection()
{
    float x, y;

    x = right * 1.f - left * 1.f;
    y = down * 1.f - up * 1.f;

    auto vector = sf::Vector2f(x, y);
    linalg::normalize(vector);

    // rewrite!
    return vector;
}


void Controls::setLastActionTimepoint(intmax_t t)
{
    last_action_timepoint = t;
}


intmax_t Controls::getDeltaTime()
{
    intmax_t t = std::chrono::steady_clock::now().time_since_epoch().count();
    intmax_t dt = t - last_action_timepoint;
    //setLastActionTimepoint(t);

    return dt;
}