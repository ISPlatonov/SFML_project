#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Constants.hpp"
#include "Multiplayer.hpp"
#include "Object.hpp"
#include "Hash.hpp"
#include <vector>
#include <unordered_map>
#include <chrono>


namespace WorldMap
{
    typedef std::unordered_map<sf::Vector2f, Object::Object> LayerMap;

    /**
     * @brief Class for storing map data
     * @details Stores map data, such as objects, sectors, etc.
    */
    class ObjectMap
    {
    public:
        ObjectMap() {}
        LayerMap& getObjectMap(const Object::Passability&);
        void addObject(const Multiplayer::ObjectData&);
        void removeObject(const Multiplayer::ObjectData&);

    private:
        LayerMap background_objects;
        LayerMap foreground_objects;
        LayerMap impassible_objects;
    };
}