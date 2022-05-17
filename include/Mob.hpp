#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Actor.hpp"


namespace
{
    std::map<std::string, sf::Texture> load_textures(std::string texture_dir_path)
    {
        // Load a sprite to display
        std::map<std::string, sf::Texture> textures;
        textures["left"] = sf::Texture();
        textures["right"] = sf::Texture();
        textures["left"].loadFromFile(texture_dir_path + "/left.png");
        textures["right"].loadFromFile(texture_dir_path + "/right.png");

        return textures;
    }
}


namespace Mob
{
    std::vector<Bot> mob_list;

    static User user(load_textures("textures/actors/Guy_16x32"), sf::Vector2f(0, 0) * static_cast<float>(PIXEL_SIZE));
}