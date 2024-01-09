#include "WorldMap.hpp"


namespace WorldMap
{
    LayerMap& ObjectMap::getObjectMap(const Object::Passability& passability)
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
                throw std::invalid_argument("invalid passability");
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