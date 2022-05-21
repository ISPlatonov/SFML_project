#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "LinearAlgebra.hpp"
#include <chrono>


class Controls
{
public:
    // set ::now() as last action timepoint
    static void setLastActionTimepoint(sf::Uint32 t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

    // get dt = ::now() - last_action_timepoint
    // then reset the time
    static sf::Uint32 getDeltaTime();
    static void addEvent(sf::Event);
    static sf::Vector2f getDirection();

private:
    static sf::Vector2f direction;
    //sf::Vector2f key_to_direction(sf::Keyboard::Key key);
    static bool left, right, up, down;
    static sf::Uint32 last_action_timepoint;
};
