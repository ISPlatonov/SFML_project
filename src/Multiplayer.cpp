#include "Multiplayer.hpp"


sf::Packet& operator <<(sf::Packet& packet, const Object::Object& object)
{
    auto position = object.getPosition() / static_cast<float>(PIXEL_SIZE);
    sf::Uint32 time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    packet << position.x << position.y << time << object.getName() << object.getPassability();
    return packet;
}


Object::Object& operator <<(Object::Object& object, const Multiplayer::ObjectData& object_data)
{
    auto position = object_data.getPosition() * static_cast<float>(PIXEL_SIZE);
    auto name = object_data.getName();
    auto passability = object_data.getPassability();
    object = Object::Object(name, position, passability);
    return object;
}


namespace Multiplayer
{
    Transportable::Transportable()
    {
        sent_time = 0;
    }


    PlayerData::PlayerData() : Transportable::Transportable()
    {

    }


    Transportable::Transportable(sf::Vector2f position, sf::Uint32 sent_time)
    {
        this->position = position;
        this->sent_time = sent_time;
    }


    PlayerData::PlayerData(const PlayerData& player) : Transportable::Transportable(player.getPosition(), player.getTime())
    {
        ip = player.getIp();
        local_ip = player.getLocalIp();
        inventory = player.getInventory();
    }
        
        
    PlayerData::PlayerData(const sf::Vector2f& position, const int& ip, const int& local_ip, const sf::Uint32& time, const std::unordered_map<Object::ObjectName, size_t>& new_inventory) : Transportable::Transportable(position, time)
    {
        this->ip = ip;
        this->local_ip = local_ip;
        inventory = new_inventory;
    }


    ObjectData::ObjectData() : Transportable::Transportable()
    {

    }


    ObjectData::ObjectData(const ObjectData& object) : Transportable::Transportable(object.getPosition(), object.getTime())
    {
        object_name = object.getName();
        passability = object.getPassability();
    }


    ObjectData::ObjectData(sf::Vector2f position, sf::Uint32 time, Object::ObjectName name, Object::Passability passability) : Transportable::Transportable(position, time)
    {
        this->object_name = name;
        this->passability = passability;
    }


    void Transportable::setPosition(const sf::Vector2f& new_position)
    {
        position = new_position;
    }
    
    
    void Transportable::setTime(const sf::Uint32& new_time)
    {
        sent_time = new_time;
    }
        
        
    const::sf::Vector2f& Transportable::getPosition() const
    {
        return position;
    }
    
    
    const int& PlayerData::getIp() const
    {
        return ip;
    }
    
    
    const int& PlayerData::getLocalIp() const
    {
        return local_ip;
    }


    const size_t PlayerData::objectNumber(Object::ObjectName name) const
    {
        if (!inventory.count(name))
            return 0;
        else
            return inventory.at(name);
    }


    const size_t PlayerData::addObject(Object::ObjectName name)
    {
        if (!inventory.count(name))
            inventory[name] = 1;
        else
            inventory[name] += 1;
        return objectNumber(name);
    }


    const size_t PlayerData::removeObject(Object::ObjectName name)
    {
        if (!inventory.count(name))
            return 0;
        else
            inventory[name] -= 1;
        auto num = objectNumber(name);
        if (!num)
            inventory.erase(inventory.find(name));
        return num;
    }


    const std::unordered_map<Object::ObjectName, size_t>& PlayerData::getInventory() const
    {
        return inventory;
    }


    const sf::Uint32& Transportable::getTime() const
    {
        return sent_time;
    }


    const Object::ObjectName& ObjectData::getName() const
    {
        return object_name;
    }


    const Object::Passability& ObjectData::getPassability() const
    {
        return passability;
    }


    UdpManager::UdpManager(const sf::IpAddress& address_receive, const sf::IpAddress& address_send)
    {
        this->address_receive = sf::IpAddress(address_receive);
        this->address_send = sf::IpAddress(address_send);
        port = PORT_LISTEN;
        port_send = PORT_SEND;

        unsigned short port = PORT_LISTEN;
        unsigned short port_send = PORT_SEND;
        auto broadcast_ip = SERVER_IP;
        ip = sf::IpAddress::getPublicAddress(sf::seconds(MAX_PING));
        local_ip = sf::IpAddress::getLocalAddress();
        socket.setBlocking(false);
        // bind the socket to a port
        if (socket.bind(port) != sf::Socket::Done)
            delete this;
        if (socket_send.bind(port_send) != sf::Socket::Done)
            delete this;
    }


    sf::Socket::Status UdpManager::receive()
    {
        sf::Packet data;
        sf::Uint32 sent_time;
        sf::Vector2f new_position;
        auto status = socket.receive(data, address_receive, port_send);
        if (status != sf::Socket::Status::Done)
            return status;
        DataType data_type;
        sf::Uint32 data_type_enum;
        data >> data_type_enum;
        data_type = static_cast<DataType>(data_type_enum);

        switch (data_type)
        {
            case DataType::Object:
            {
                // receive object
                Object::ObjectName object_name;
                Object::Passability passability;
                sf::Uint32 object_name_enum;
                sf::Uint32 passability_enum;
                data >> new_position.x >> new_position.y >> sent_time >> object_name_enum >> passability_enum;
                object_name = static_cast<Object::ObjectName>(object_name_enum);
                passability = static_cast<Object::Passability>(passability_enum);
                addObject(ObjectData(std::move(new_position), std::move(sent_time), std::move(object_name), std::move(passability)));
                break;
            }
            case DataType::Player:
            {
                int msg_local_ip, msg_ip;
                data >> new_position.x >> new_position.y >> msg_ip >> msg_local_ip >> sent_time;
                sf::Uint32 inventory_size_uint32;
                data >> inventory_size_uint32;
                size_t inventory_size = static_cast<size_t>(inventory_size_uint32);
                std::unordered_map<Object::ObjectName, size_t> inventory;
                for (size_t i = 0; i < inventory_size; ++i)
                {
                    sf::Uint32 object_name_enum;
                    sf::Uint32 object_num_uint32;
                    data >> object_name_enum >> object_num_uint32;
                    size_t object_num = static_cast<size_t>(object_num_uint32);
                    inventory[static_cast<Object::ObjectName>(object_name_enum)] = std::move(object_num);
                }
                //if (msg_ip == my_ip.toInteger() && msg_local_ip == my_local_ip.toInteger())
                //    continue;
                auto id = sf::IpAddress(msg_ip).toString() + sf::IpAddress(msg_local_ip).toString();
                sf::Uint32 time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                int ping = static_cast<int>(time_now) - static_cast<int>(sent_time);
                if (player_data_pool.count(id))
                    if (ping > MAX_PING)
                        player_data_pool.erase(id);
                    else
                    {
                        player_data_pool[id].setPosition(new_position);
                        player_data_pool[id].setTime(sent_time);
                    }
                else
                    if (ping > MAX_PING)
                        return status;
                    else
                        player_data_pool[id] = PlayerData(std::move(new_position), std::move(msg_ip), std::move(msg_local_ip), std::move(sent_time), std::move(inventory));
                break;
            }
            case DataType::Event:
            {
                // event handling
                sf::Uint32 event_type_enum;
                data >> event_type_enum;
                EventType event_type = static_cast<EventType>(event_type_enum);
                switch (event_type)
                {
                    case EventType::takeObjectToInventory:
                    {
                        // data >> object_data >> user
                        // receive object
                        Object::ObjectName object_name;
                        Object::Passability passability;
                        sf::Uint32 object_name_enum;
                        sf::Uint32 passability_enum;
                        data >> new_position.x >> new_position.y >> sent_time >> object_name_enum >> passability_enum;
                        object_name = static_cast<Object::ObjectName>(object_name_enum);
                        passability = static_cast<Object::Passability>(passability_enum);
                        auto object = ObjectData(std::move(new_position), std::move(sent_time), std::move(object_name), std::move(passability));
                        // receive user
                        int msg_local_ip, msg_ip;
                        data >> new_position.x >> new_position.y >> msg_ip >> msg_local_ip >> sent_time;
                        sf::Uint32 inventory_size_uint32;
                        data >> inventory_size_uint32;
                        size_t inventory_size = static_cast<size_t>(inventory_size_uint32);
                        std::unordered_map<Object::ObjectName, size_t> inventory;
                        for (size_t i = 0; i < inventory_size; ++i)
                        {
                            sf::Uint32 object_name_enum;
                            sf::Uint32 object_num_uint32;
                            data >> object_name_enum >> object_num_uint32;
                            size_t object_num = static_cast<size_t>(object_num_uint32);
                            inventory[static_cast<Object::ObjectName>(object_name_enum)] = std::move(object_num);
                        }
                        auto id = sf::IpAddress(msg_ip).toString() + sf::IpAddress(msg_local_ip).toString();
                        sf::Uint32 time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                        int ping = static_cast<int>(time_now) - static_cast<int>(sent_time);
                        if (player_data_pool.count(id))
                            if (ping > MAX_PING)
                                player_data_pool.erase(id);
                            else
                            {
                                player_data_pool[id].setPosition(new_position);
                                player_data_pool[id].setTime(sent_time);
                                player_data_pool[id].addObject(object_name);
                                removeObjectByPoint(object.getPosition());
                                data.clear();
                                data << DataType::Event << EventType::removeObject << object.getPosition().x << object.getPosition().y << object.getTime() << object.getName() << object.getPassability();
                                for (auto iter = getPlayerDataPool().begin(); iter != getPlayerDataPool().end(); ++iter)
                                    send(data, sf::IpAddress((*iter).second.getLocalIp()));
                            }
                        else
                            if (ping > MAX_PING)
                                return status;
                            else
                            {
                                player_data_pool[id] = PlayerData(std::move(new_position), std::move(msg_ip), std::move(msg_local_ip), std::move(sent_time), std::move(inventory));
                                player_data_pool[id].addObject(object.getName());
                                removeObjectByPoint(object.getPosition());
                                data.clear();
                                data << DataType::Event << EventType::removeObject << object.getPosition().x << object.getPosition().y << object.getTime() << object.getName() << object.getPassability();
                                for (auto iter = getPlayerDataPool().begin(); iter != getPlayerDataPool().end(); ++iter)
                                    send(data, sf::IpAddress((*iter).second.getLocalIp()));
                            }
                        break;
                    }
                    case EventType::ejectObjectFromInventory:
                        //
                        break;
                    
                    case EventType::removeObject:
                    {
                        // receive object
                        Object::ObjectName object_name;
                        Object::Passability passability;
                        sf::Uint32 object_name_enum;
                        sf::Uint32 passability_enum;
                        data >> new_position.x >> new_position.y >> sent_time >> object_name_enum >> passability_enum;
                        object_name = static_cast<Object::ObjectName>(object_name_enum);
                        passability = static_cast<Object::Passability>(passability_enum);
                        removeObjectByPoint(new_position);
                        break;
                    }
                    default:
                        throw; // ???
                        break;
                }
                break;
            }
            default:
                throw; // ???
                break;
        }
        return status;
    }


    const std::unordered_map<std::string, PlayerData>& UdpManager::getPlayerDataPool() const
    {
        return player_data_pool;
    }


    const std::unordered_map<sf::Vector2f, ObjectData>& UdpManager::getObjectDataPool() const
    {
        return object_data_pool;
    }


    std::unordered_map<std::string, PlayerData>::iterator UdpManager::removePlayerById(const std::string& id)
    {
        if (!player_data_pool.count(id))
            return std::unordered_map<std::string, PlayerData>::iterator();
        else
            return player_data_pool.erase(player_data_pool.find(id));
    }


    std::unordered_map<sf::Vector2f, ObjectData>::iterator UdpManager::removeObjectByPoint(const sf::Vector2f& point)
    {
        if (!object_data_pool.count(point))
            return std::unordered_map<sf::Vector2f, ObjectData>::iterator();
        else
        {
            removed_object_data_list.push_back((*object_data_pool.find(point)).second);
            return object_data_pool.erase(object_data_pool.find(point));
        }
    }


    void UdpManager::addObject(const Object::Object& object)
    {
        ObjectData object_data(object.getPosition() / static_cast<float>(PIXEL_SIZE), static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()), object.getName(), object.getPassability());
        object_data_pool[object_data.getPosition()] = object_data;
    }


    void UdpManager::addObject(const Multiplayer::ObjectData& object_data)
    {
        object_data_pool[object_data.getPosition()] = object_data;
    }


    ObjectData&& UdpManager::getRemovedObjectData()
    {
        if (removed_object_data_list.empty())
        {
            ObjectData* object_data = new ObjectData();
            return std::move(*object_data);
        }
        else
        {
            ObjectData& object_data = removed_object_data_list.back();
            removed_object_data_list.pop_back();
            return std::move(object_data);
        }
    }


    void UdpManager::send(sf::Packet& packet, const sf::IpAddress& dest_ip)
    {
        if (dest_ip == sf::IpAddress())
            socket_send.send(packet, address_send, port);
        socket_send.send(packet, dest_ip, port);
    }
}