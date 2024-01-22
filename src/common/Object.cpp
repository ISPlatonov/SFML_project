#include "Object.hpp"


namespace Object
{
    std::unordered_map<ObjectName, sf::Texture> Object::NameToTextureMap{};


    const std::unordered_map<ObjectName, std::string> Object::NameToTextureAddressMap
    {
        std::make_pair(ObjectName::apple, "textures/objects/apple/texture.png"),
        std::make_pair(ObjectName::wooden_wall, "textures/objects/wooden_wall/texture.png"),
        std::make_pair(ObjectName::grass, "textures/objects/grass/texture.png"),
        std::make_pair(ObjectName::dirt, "textures/objects/dirt/texture.png"),
        std::make_pair(ObjectName::stone, "textures/objects/stone/texture.png"),
        std::make_pair(ObjectName::dirty_grass, "textures/objects/dirty_grass/texture.png")
    };


    Object::Object(const ObjectName& name, const sf::Vector2f& new_position, const Passability& new_passability)
    {
        this->name = name;
        if (!NameToTextureMap.count(name))
        {
            auto new_texture = sf::Texture();
            new_texture.loadFromFile(NameToTextureAddressMap.at(name));
            NameToTextureMap[name] = std::move(new_texture);
        }
        position = new_position;
        passability = new_passability;
        sprite.setScale(Constants::getTILE_SIZE(), Constants::getTILE_SIZE());
        sprite.setTexture(NameToTextureMap.at(name));
        sprite.setPosition(position);
        // need a fix
    }


    Object::Object(ObjectName&& name, sf::Vector2f&& new_position, Passability&& new_passability)
    {
        this->name = std::move(name);
        if (!NameToTextureMap.count(name))
        {
            auto new_texture = sf::Texture();
            new_texture.loadFromFile(NameToTextureAddressMap.at(name));
            NameToTextureMap[name] = std::move(new_texture);
        }
        position = std::move(new_position);
        passability = std::move(new_passability);
        sprite.setScale(Constants::getTILE_SIZE(), Constants::getTILE_SIZE());
        sprite.setTexture(NameToTextureMap.at(name));
        sprite.setPosition(position);
        // need a fix
    }


    void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &NameToTextureMap.at(name);

        // draw the vertex array
        target.draw(sprite, states);
    }


    void Object::check_collision(sf::Vector2f& vector, const sf::FloatRect& rect, size_t& depth) const
    {
        if (this->intersection(sf::FloatRect(rect.getPosition() + vector, rect.getSize())))
        {
            if (!this->intersection(sf::FloatRect(rect.getPosition() + sf::Vector2f(vector.x, 0), rect.getSize())))
            {
                if (depth < Constants::getMAX_RECURSION_DEPTH())
                    vector.y /= 2;
                else
                    vector.y = 0;
            }
            else if (!this->intersection(sf::FloatRect(rect.getPosition() + sf::Vector2f(0, vector.y), rect.getSize())))
            {
                if (depth < Constants::getMAX_RECURSION_DEPTH())
                    vector.x /= 2;
                else
                    vector.x = 0;
            }
            else
            {
                if (depth < Constants::getMAX_RECURSION_DEPTH())
                {
                    vector.x /= 2;
                    vector.y /= 2;
                }
                else
                {
                    vector.x = 0;
                    vector.y = 0;
                    return;
                }
            }
            this->check_collision(vector, rect, ++depth);
        }
        else
            return;
    }


    void Object::check_collision(sf::Vector2f& vector, const sf::FloatRect& rect) const
    {
        if (passability != Passability::impassible)
            return;
        size_t depth = 0;
        check_collision(vector, rect, depth);
    }
}