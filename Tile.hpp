#pragma once

#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class Tile
{
    sf::Sprite sprite;
    sf::Vector2f position;
    bool is_passible;
    
public:
    Tile(std::string sprite_path, sf::Vector2f position, bool is_passible);
    const bool isPassible();
    const sf::Vector2f getPosition();
};