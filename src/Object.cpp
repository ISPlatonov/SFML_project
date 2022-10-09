#include "Object.hpp"


namespace Object
{
    std::map<ObjectName, std::string> Object::NameToTextureMap
    {
        {ObjectName::apple, "textures/objects/apple"},
        {ObjectName::grass, "textures/objects/grass"},
        {ObjectName::wooden_wall, "textures/objects/wooden_wall"}
    };


    Object::Object()
    {

    }


    Object::Object(const std::string& texture_address, const sf::Vector2f& new_position, const Passability& new_passability)
    {
        if (!this->texture.loadFromFile(texture_address + "/texture.png"))
            delete this;
        position = new_position;
        passability = new_passability;
        sprite.setScale(TILE_SIZE, TILE_SIZE);
        sprite.setTexture(texture);
        sprite.setPosition(position);
        // need a fix
    }


    Object::Object(const sf::Texture& new_texture, const sf::Vector2f& new_position, const Passability& new_passability)
    {
        texture = new_texture;
        position = new_position;
        passability = new_passability;
        sprite.setScale(TILE_SIZE, TILE_SIZE);
        sprite.setTexture(texture);
        sprite.setPosition(position);
        // need a fix
    }


    Object::Object(sf::Texture&& new_texture, sf::Vector2f&& new_position, Passability&& new_passability)
    {
        texture = std::move(new_texture);
        position = std::move(new_position);
        passability = std::move(new_passability);
        sprite.setScale(TILE_SIZE, TILE_SIZE);
        sprite.setTexture(texture);
        sprite.setPosition(position);
        // need a fix
    }


    void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &texture;

        // draw the vertex array
        target.draw(sprite, states);
    }


    const Passability& Object::getPassability() const
    {
        return passability;
    }


    const sf::Texture& Object::getTexture() const
    {
        return texture;
    }
    
    
    const sf::Vector2f& Object::getPosition() const
    {
        return position;
    }


    const sf::Sprite& Object::getSprite() const
    {
        return sprite;
    }


    const ObjectName& Object::getName() const
    {
        return name;
    }


    bool Object::intersection(const sf::FloatRect& new_rect) const
    {
        auto bounds = getSprite().getGlobalBounds();
        bool f = bounds.intersects(new_rect);
        return f;
    }


    void Object::check_collision(sf::Vector2f& vector, const sf::FloatRect& rect, size_t& depth) const
    {
        if (this->intersection(sf::FloatRect(rect.getPosition() + vector, rect.getSize())))
        {
            if (!this->intersection(sf::FloatRect(rect.getPosition() + sf::Vector2f(vector.x, 0), rect.getSize())))
            {
                if (depth < MAX_RECURSION_DEPTH)
                    vector.y /= 2;
                else
                    vector.y = 0;
            }
            else if (!this->intersection(sf::FloatRect(rect.getPosition() + sf::Vector2f(0, vector.y), rect.getSize())))
            {
                if (depth < MAX_RECURSION_DEPTH)
                    vector.x /= 2;
                else
                    vector.x = 0;
            }
            else
            {
                if (depth < MAX_RECURSION_DEPTH)
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