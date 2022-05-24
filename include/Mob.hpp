#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Actor.hpp"


namespace Mob
{
    std::vector<Actor::Bot> mob_list;
    std::vector<Actor::Actor> multiplayers_list;

    //auto Guy_textures = load_textures("textures/actors/Guy_16x32");

    //static Actor::User user(load_textures("textures/actors/Guy_16x32"), sf::Vector2f(0, 0) * static_cast<float>(PIXEL_SIZE));
}