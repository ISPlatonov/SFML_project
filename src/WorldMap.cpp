#include "WorldMap.hpp"


namespace WorldMap
{
    TileMap::TileMap(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            delete this;

        // resize the tileset texture
        this->setScale(TILE_SIZE, TILE_SIZE);

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
    }


    void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
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
} // namespace WorldMap