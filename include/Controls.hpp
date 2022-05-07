#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "LinearAlgebra.hpp"
#include <chrono>
//#include "Constants.hpp"


class Controls
{
public:
    // set ::now() as last action timepoint
    static void setLastActionTimepoint(intmax_t t = std::chrono::steady_clock::now().time_since_epoch().count());

    // get dt = ::now() - last_action_timepoint
    // then reset the time
    static intmax_t getDeltaTime();
    static void addEvent(sf::Event);
    static sf::Vector2f getDirection();

private:
    static sf::Vector2f direction;
    //sf::Vector2f key_to_direction(sf::Keyboard::Key key);
    static bool left, right, up, down;
    static intmax_t last_action_timepoint;
};
