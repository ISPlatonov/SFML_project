#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <map>
#include <string>
#include "Constants.hpp"

#define STEP_SIZE_MULTIPLIER float(0.00000005) // ~2^-24

class Actor
{
public:
    //Actor(sf::Texture texture_left, sf::Texture texture_right);

    Actor(std::map<std::string, sf::Texture> textures, sf::Vector2f position);

    void PressedA();
    void PressedD();
    void ReleasedA();
    void ReleasedD();

    template <typename T>
    void step(T step_size);
    void step();

    //void setPosition(float x, float y);

    void setPosition(sf::Vector2f p);

    const sf::Sprite getSprite();

private:    
    bool left, right;
    sf::Sprite sprite;
    std::map<std::string, sf::Texture> textures;
    intmax_t last_action_timepoint;
};