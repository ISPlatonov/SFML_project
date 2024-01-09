#include "Multiplayer.hpp"


sf::Packet& operator <<(sf::Packet& packet, Multiplayer::ObjectData& object_data)
{
    object_data.setTime(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    packet << object_data.getPosition().x << object_data.getPosition().y << static_cast<sf::Uint32>(object_data.getTime()) << object_data.getName() << object_data.getPassability();
    return packet;
}


Object::Object& operator <<(Object::Object& object, const Multiplayer::ObjectData& object_data)
{
    auto position = object_data.getPosition() * static_cast<float>(Constants::getPIXEL_SIZE());
    auto& name = object_data.getName();
    auto& passability = object_data.getPassability();
    object = Object::Object(name, position, passability);
    return object;
}


sf::Packet& operator >>(sf::Packet& data, Multiplayer::ObjectData& object_data)
{
    sf::Vector2f position;
    Object::ObjectName object_name;
    Object::Passability passability;
    sf::Uint32 object_name_enum;
    sf::Uint32 passability_enum;
    sf::Uint32 sent_time;
    data >> position.x >> position.y >> sent_time >> object_name_enum >> passability_enum;
    object_name = static_cast<Object::ObjectName>(object_name_enum);
    passability = static_cast<Object::Passability>(passability_enum);
    object_data = Multiplayer::ObjectData(std::move(position), std::move(sent_time), std::move(object_name), std::move(passability));
    return data;
}


sf::Packet& operator <<(sf::Packet& packet, const Multiplayer::PlayerData& player_data)
{
    packet << player_data.getPosition().x << player_data.getPosition().y << player_data.getIp() << player_data.getPort() << player_data.getTime();
    packet << static_cast<sf::Uint32>(player_data.getInventory().size());
    for (auto pair : player_data.getInventory())
    {
        packet << pair.first << static_cast<sf::Uint32>(pair.second);
    }
    return packet;
}


sf::Packet& operator >>(sf::Packet& packet, Multiplayer::PlayerData& player_data)
{
    int msg_ip;
    sf::Vector2f position;
    sf::Uint32 sent_time;
    unsigned int msg_port;
    packet >> position.x >> position.y >> msg_ip >> msg_port >> sent_time;
    sf::Uint32 inventory_size_uint32;
    packet >> inventory_size_uint32;
    size_t inventory_size = static_cast<size_t>(inventory_size_uint32);
    Multiplayer::Inventory inventory;
    for (size_t i = 0; i < inventory_size; ++i)
    {
        sf::Uint32 object_name_enum;
        sf::Uint32 object_num_uint32;
        packet >> object_name_enum >> object_num_uint32;
        size_t object_num = static_cast<size_t>(object_num_uint32);
        inventory[static_cast<Object::ObjectName>(object_name_enum)] = std::move(object_num);
    }
    player_data = Multiplayer::PlayerData(std::move(position), std::move(msg_ip), std::move(msg_port), std::move(sent_time), std::move(inventory)); // no inventory needed!
    return packet;
}
