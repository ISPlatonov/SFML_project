#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <map>
#include <string>
#include "Constants.hpp"
#include "LinearAlgebra.hpp"


class Actor
{
public:
    //Actor(sf::Texture texture_left, sf::Texture texture_right);

    Actor(std::map<std::string, sf::Texture> textures, sf::Vector2f position);
    /*
    void PressedA();
    void PressedD();
    void PressedW();
    void PressedS();

    void ReleasedA();
    void ReleasedD();
    void ReleasedW();
    void ReleasedS();

    template <typename T>
    void step(T step_size);
    void step();
    */

    //void setPosition(float x, float y);

    void move(sf::Vector2f direction, intmax_t dt);

    void setPosition(sf::Vector2f p);

    const sf::Sprite getSprite();

    sf::View view;

private:    
    //bool left, right, up, down;
    sf::Sprite sprite;
    std::map<std::string, sf::Texture> textures;
    //intmax_t last_action_timepoint;
};