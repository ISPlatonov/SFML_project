#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include <vector>
#include <map>


namespace WorldMap
{
    enum Passability
    {
        impassible,
        background,
        foreground
    };


    class TileMap : public sf::Drawable, public sf::Transformable
    {
    public:
        TileMap(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);
        
    private:
        sf::VertexArray m_vertices;
        sf::Texture m_tileset;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };


    class Object : public sf::Drawable, public sf::Transformable
    {
    public:
        Object(const std::string& texture_address, const sf::Vector2f& position, const Passability& passability);
        Object(const sf::Texture& texture, const sf::Vector2f& position, const Passability& passability);
        Object(sf::Texture&& texture, sf::Vector2f&& position, Passability&& passability);
        Object(const Object& object) : Object(object.getTexture(), object.getPosition(), object.getPassability()) {};
        //Object(std::string&& texture_address, sf::Vector2f&& position, Passability&& passability);
        //Object(Object&& object) : Object(std::move(object.texture), std::move(object.position), std::move(object.passability)) {};
        const Passability& getPassability() const;
        const sf::Texture& getTexture() const;
        const sf::Vector2f& getPosition() const;
        const sf::Sprite& getSprite() const;
        //void checkCollision(sf::Vector2f& vector);
        //std::pair<Passability, Object> operatop
        
        // think about it
        void check_collision(sf::Vector2f& vector, const sf::FloatRect& rect) const;

    protected:
        sf::Texture texture;
        sf::Vector2f position;
        sf::Sprite sprite;
        Passability passability;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        bool intersection(const sf::FloatRect& rect) const;
        void check_collision(sf::Vector2f& vector, const sf::FloatRect& rect, size_t& depth) const;
    };


    using ObjectMap = std::multimap<Passability, Object>;


    class WorldMap
    {
    public:
        static TileMap map;

    private:
        WorldMap();
    };
}