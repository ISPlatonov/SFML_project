#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

#define STEP_SIZE_MULTIPLIER float(0.000001)

class Actor
{
public:
    Actor(sf::Sprite sprite);

    void PressedA();

    void PressedD();

    void ReleasedA();

    void ReleasedD();

    template <typename T>
    void step(T step_size);

    void step();

    const sf::Sprite getSprite();

private:    
    bool left, right;
    sf::Sprite sprite;
    intmax_t last_action_timepoint;
};