#include "WorldMap.hpp"


namespace WorldMap
{
    // define the level with an array of tile indices
    const int level[] =
    {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 1, 1, 2, 2, 2,
        2, 2, 1, 2, 32, 32, 1, 2,
        2, 2, 1, 1, 1, 1, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 207, 207, 2, 2, 2,
        2, 2, 2, 2, 207, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 1, 1, 2, 2, 2,
        2, 2, 1, 2, 32, 32, 1, 2,
        2, 2, 1, 1, 1, 1, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 207, 207, 2, 2, 2,
        2, 2, 2, 2, 207, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
    };

    TileMap WorldMap::map("textures/terrain.png", sf::Vector2u(16, 16), level, 8, 16);


    TileMap::TileMap(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            delete this;

        // resize the tileset texture
        this->setScale(Constants::getTILE_SIZE(), Constants::getTILE_SIZE());

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


    WorldMap::WorldMap(const sf::Vector2u& size)
    {
        //int level_map[size.y][size.x];
        for (auto y = 0; y < size.y; ++y)
        {
            for (auto x = 0; x < size.x; ++x)
            {
                //level_map[y][x] = std::uniform_int_distribution<int>(0, )
                //ObjectMap.append())
            }
        }
    }


    ObjectMap::ObjectMap()
    {

    }


    const std::unordered_map<sf::Vector2f, Object::Object>& ObjectMap::getObjectMap(const Object::Passability& passability) const
    {
        switch (passability)
        {
            case Object::Passability::background:
                return background_objects;
                break;

            case Object::Passability::foreground:
                return foreground_objects;
                break;

            case Object::Passability::impassible:
                return impassible_objects;
                break;

            default:
                throw;
                break;
        }
    }


    void ObjectMap::addObject(const Multiplayer::ObjectData& object_data)
    {
        Object::Object object;
        object << object_data;
        auto point = object.getPosition();
        switch (object.getPassability())
        {
            case Object::Passability::background:
                if (!background_objects.count(point) || background_objects[point].getName() != object.getName())
                    background_objects[point] = object;
                break;
            case Object::Passability::foreground:
                if (!foreground_objects.count(point) || foreground_objects[point].getName() != object.getName())
                    foreground_objects[point] = object;
                break;
            case Object::Passability::impassible:
                if (!impassible_objects.count(point) || impassible_objects[point].getName() != object.getName())
                    impassible_objects[point] = object;
                break;
        }
    }


    void ObjectMap::removeObject(const Multiplayer::ObjectData& object_data)
    {
        auto point = object_data.getPosition() * static_cast<float>(Constants::getPIXEL_SIZE());
        switch (object_data.getPassability())
        {
            case Object::Passability::background:
            {
                if (background_objects.count(point) && background_objects[point].getName() == object_data.getName())
                    background_objects.erase(point);
                break;
            }
            case Object::Passability::foreground:
            {
                if (foreground_objects.count(point) && foreground_objects[point].getName() == object_data.getName())
                    foreground_objects.erase(point);
                break;
            }
            case Object::Passability::impassible:
            {
                if (impassible_objects.count(point) && impassible_objects[point].getName() == object_data.getName())
                    impassible_objects.erase(point);
                break;
            }
            default:
            {
                throw; // ???
                break;
            }
        }
    }
} // namespace WorldMap