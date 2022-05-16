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
    Actor(std::map<std::string, sf::Texture> textures, sf::Vector2f position);
    sf::View view;

    void move(sf::Vector2f direction, intmax_t dt);
    void setPosition(sf::Vector2f p);
    void check_direction(sf::Vector2f);
    const sf::Sprite getSprite();

private:    
    sf::Sprite sprite;
    std::map<std::string, sf::Texture> textures;
    std::string direction_x;
};