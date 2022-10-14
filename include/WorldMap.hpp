#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Constants.hpp"
#include "Multiplayer.hpp"
#include "Object.hpp"
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <iostream>


namespace WorldMap
{
    class TileMap : public sf::Drawable, public sf::Transformable
    {
    public:
        TileMap(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
        
    private:
        sf::VertexArray m_vertices;
        sf::Texture m_tileset;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };


    class ObjectMap
    {
    public:
        ObjectMap();
        const std::map<std::pair<float, float>, Object::Object>& getObjectMap(Object::Passability) const;
        void addObject(Multiplayer::ObjectData);

    private:
        std::map<std::pair<float, float>, Object::Object> background_objects;
        std::map<std::pair<float, float>, Object::Object> foreground_objects;
        std::map<std::pair<float, float>, Object::Object> impassible_objects;
    };


    class WorldMap
    {
    public:
        WorldMap(const sf::Vector2u& size);
        static TileMap map;

    private:
        WorldMap();
    };
}