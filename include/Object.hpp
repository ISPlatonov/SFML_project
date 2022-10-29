#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include <unordered_map>
#include <vector>
#include <string>


namespace Object
{
    enum Passability
    {
        impassible,
        background,
        foreground
    };


    enum ObjectName
    {
        apple,
        wooden_wall,
        grass,
        dirt,
        stone,
        dirty_grass
    };


    /**
     * @brief Class for storing object data
     * @details Stores object data, such as name, texture, passability, etc.
    */
    class Object : public sf::Drawable, public sf::Transformable
    {
    public:
        Object();
        Object(const ObjectName& name, const sf::Vector2f& position, const Passability& passability);
        Object(ObjectName&& name, sf::Vector2f&& position, Passability&& passability);
        Object(const Object& object) : Object(object.getName(), object.getPosition(), object.getPassability()) {};
        //Object(std::string&& texture_address, sf::Vector2f&& position, Passability&& passability);
        //Object(Object&& object) : Object(std::move(object.texture), std::move(object.position), std::move(object.passability)) {};
        const Passability& getPassability() const;
        const sf::Vector2f& getPosition() const;
        const sf::Sprite& getSprite() const;
        const ObjectName& getName() const;
        // think about it
        void check_collision(sf::Vector2f& vector, const sf::FloatRect& rect) const;
        static std::unordered_map<ObjectName, sf::Texture> NameToTextureMap;
        static const std::unordered_map<ObjectName, std::string> NameToTextureAddressMap;

    protected:
        sf::Vector2f position;
        sf::Sprite sprite;
        Passability passability;
        ObjectName name;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        bool intersection(const sf::FloatRect& rect) const;
        void check_collision(sf::Vector2f& vector, const sf::FloatRect& rect, size_t& depth) const;
    };
} // namespace Object