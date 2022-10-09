#pragma once

#include <map>
#include <vector>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Constants.hpp"


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
        grass
    };


    class Object : public sf::Drawable, public sf::Transformable
    {
    public:
        Object();
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
        const ObjectName& getName() const;
        // think about it
        void check_collision(sf::Vector2f& vector, const sf::FloatRect& rect) const;
        static std::map<ObjectName, std::string> NameToTextureMap;

    protected:
        sf::Texture texture;
        sf::Vector2f position;
        sf::Sprite sprite;
        Passability passability;
        ObjectName name;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        bool intersection(const sf::FloatRect& rect) const;
        void check_collision(sf::Vector2f& vector, const sf::FloatRect& rect, size_t& depth) const;
    };
} // namespace Object