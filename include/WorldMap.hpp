#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Constants.hpp"
#include "Multiplayer.hpp"
#include "Object.hpp"
#include "Hash.hpp"
#include <vector>
#include <unordered_map>
#include <random>
#include <chrono>


namespace WorldMap
{
    /**
     * @brief Class for storing tiles
     * @todo get rid of this class
    */
    class TileMap : public sf::Drawable, public sf::Transformable
    {
    public:
        TileMap(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
        
    private:
        sf::VertexArray m_vertices;
        sf::Texture m_tileset;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };


    /**
     * @brief Class for storing map data
     * @details Stores map data, such as objects, sectors, etc.
    */
    class ObjectMap
    {
    public:
        ObjectMap();
        std::unordered_map<sf::Vector2f, Object::Object>& getObjectMap(const Object::Passability&);
        void addObject(const Multiplayer::ObjectData&);
        void removeObject(const Multiplayer::ObjectData&);

    private:
        std::unordered_map<sf::Vector2f, Object::Object> background_objects;
        std::unordered_map<sf::Vector2f, Object::Object> foreground_objects;
        std::unordered_map<sf::Vector2f, Object::Object> impassible_objects;
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