#include "Multiplayer.hpp"


sf::Packet& operator <<(sf::Packet& packet, Multiplayer::ObjectData& object_data)
{
    //#ifndef CLIENT
        object_data.setTime(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    //#endif
    packet << object_data.getPosition().x << object_data.getPosition().y << static_cast<sf::Uint32>(object_data.getTime()) << object_data.getName() << object_data.getPassability();
    return packet;
}


Object::Object& operator <<(Object::Object& object, const Multiplayer::ObjectData& object_data)
{
    auto position = object_data.getPosition() * static_cast<float>(Constants::getPIXEL_SIZE());
    auto name = object_data.getName();
    auto passability = object_data.getPassability();
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
    packet << player_data.getPosition().x << player_data.getPosition().y << player_data.getIp() << player_data.getLocalIp() << player_data.getTime();
    packet << static_cast<sf::Uint32>(player_data.getInventory().size());
    for (auto pair : player_data.getInventory())
    {
        packet << pair.first << static_cast<sf::Uint32>(pair.second);
    }
    return packet;
}


sf::Packet& operator >>(sf::Packet& packet, Multiplayer::PlayerData& player_data)
{
    int msg_local_ip, msg_ip;
    sf::Vector2f position;
    sf::Uint32 sent_time;
    packet >> position.x >> position.y >> msg_ip >> msg_local_ip >> sent_time;
    sf::Uint32 inventory_size_uint32;
    packet >> inventory_size_uint32;
    size_t inventory_size = static_cast<size_t>(inventory_size_uint32);
    std::unordered_map<Object::ObjectName, size_t> inventory;
    for (size_t i = 0; i < inventory_size; ++i)
    {
        sf::Uint32 object_name_enum;
        sf::Uint32 object_num_uint32;
        packet >> object_name_enum >> object_num_uint32;
        size_t object_num = static_cast<size_t>(object_num_uint32);
        inventory[static_cast<Object::ObjectName>(object_name_enum)] = std::move(object_num);
    }
    player_data = Multiplayer::PlayerData(std::move(position), std::move(msg_ip), std::move(msg_local_ip), std::move(sent_time), std::move(inventory)); // no inventory needed!
    return packet;
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
        port = Constants::getPORT_LISTEN();
        port_send = Constants::getPORT_SEND();

        unsigned short port = Constants::getPORT_LISTEN();
        unsigned short port_send = Constants::getPORT_SEND();
        auto broadcast_ip = Constants::getSERVER_IP();
        ip = sf::IpAddress::getPublicAddress(sf::seconds(Constants::getMAX_PING()));
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
                Multiplayer::ObjectData object_data;
                data >> object_data;
                addObject(object_data);
                break;
            }
            case DataType::Player:
            {
                PlayerData player_data;
                data >> player_data;
                //if (msg_local_ip == local_ip.toInteger())
                //{
                //    std::cout << "its me" << std::endl;
                //}
                auto id = sf::IpAddress(player_data.getIp()).toString() + sf::IpAddress(player_data.getLocalIp()).toString();
                sf::Uint32 time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                int ping = static_cast<int>(time_now) - static_cast<int>(player_data.getTime());
                if (player_data_pool.count(id))
                {
                    #ifndef CLIENT
                        auto old_time = player_data_pool[id].getTime();
                        int old_ping = static_cast<int>(time_now) - static_cast<int>(old_time);
                        if (old_ping > Constants::getMAX_PING())
                        {
                            // send foreground objects on map
                            for (auto iter : object_data_pool)
                            {
                                for (auto obj_data : iter.second)
                                {
                                    if (obj_data.getPassability() != Object::Passability::foreground)
                                        continue;
                                    data.clear();
                                    data << DataType::Object << obj_data;
                                    send(data, sf::IpAddress(player_data.getLocalIp()));
                                    sf::sleep(sf::milliseconds(1));
                                }
                            }
                            if (player_data.getInventory().empty() && !player_data_pool[id].getInventory().empty())
                            {
                                // send all inventory
                                for (auto iter : player_data_pool[id].getInventory())
                                {
                                    for (size_t i = 0; i < iter.second; ++i)
                                    {
                                        data.clear();
                                        auto object_data = ObjectData(sf::Vector2f(0, 0), old_time, iter.first, Object::Passability::foreground);
                                        data << DataType::Event << EventType::addObjectToInvectory << object_data;
                                        send(data, sf::IpAddress(player_data.getLocalIp()));
                                    }
                                }
                            }
                            else
                                for (auto iter : player_data_pool[id].getInventory())
                                {
                                    size_t msg_number;
                                    if (player_data.getInventory().count(iter.first) && player_data.getInventory().at(iter.first) >= iter.second)
                                    {
                                        // now it does nothing,
                                        // but it has to send
                                        // every losed object
                                        // in inventory
                                    }
                                }
                        }
                    #endif
                    if (ping > Constants::getMAX_PING())
                    {
                        #ifdef CLIENT
                            player_data_pool.erase(id);
                        #endif
                    }
                    else
                    {
                        player_data_pool[id].setPosition(player_data.getPosition());
                        player_data_pool[id].setTime(player_data.getTime());
                    }
                }
                else
                    if (ping > Constants::getMAX_PING())
                        return status;
                    else
                    {
                        player_data_pool[id] = std::move(player_data);
                        // send foreground objects
                        for (auto iter : object_data_pool)
                        {
                            for (auto object_data : iter.second)
                            {
                                if (object_data.getPassability() != Object::Passability::foreground)
                                    continue;
                                data.clear();
                                data << DataType::Object << object_data;
                                send(data, sf::IpAddress(player_data.getLocalIp()));
                                sf::sleep(sf::milliseconds(1));
                            }
                        }
                    }
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
                        ObjectData object_data;
                        data >> object_data;
                        // receive user
                        PlayerData player_data;
                        data >> player_data;
                        auto id = sf::IpAddress(player_data.getIp()).toString() + sf::IpAddress(player_data.getLocalIp()).toString();
                        sf::Uint32 time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                        int ping = static_cast<int>(time_now) - static_cast<int>(player_data.getTime());
                        if (player_data_pool.count(id))
                            if (ping > Constants::getMAX_PING())
                            {
                                //player_data_pool.erase(id);
                            }
                            else
                            {
                                player_data_pool[id].setPosition(player_data.getPosition());
                                player_data_pool[id].setTime(player_data.getTime());
                                player_data_pool[id].addObject(object_data.getName());
                                removeObject(object_data);
                                data.clear();
                                data << DataType::Event << EventType::removeObject << object_data;
                                for (auto iter = getPlayerDataPool().begin(); iter != getPlayerDataPool().end(); ++iter)
                                    send(data, sf::IpAddress((*iter).second.getLocalIp()));
                                data.clear();
                                data << DataType::Event << EventType::addObjectToInvectory << object_data;
                                send(data, sf::IpAddress(player_data.getLocalIp()));
                            }
                        break;
                    }
                    case EventType::ejectObjectFromInventory:
                        //
                        break;
                    
                    case EventType::addObjectToInvectory:
                    {
                        // receive object
                        ObjectData object_data;
                        data >> object_data;
                        objects_to_inventory_list.push_back(object_data);
                        break;
                    }
                    case EventType::removeObject:
                    {
                        // receive object
                        ObjectData object_data;
                        data >> object_data;
                        removeObject(object_data);
                        break;
                    }
                    default:
                        throw; // ???
                        break;
                }
                break;
            }
            case DataType::Sector:
            {
                // data >> objects_num >> objects >> ...
                sf::Uint32 objects_number;
                data >> objects_number;
                for (size_t i = 0; i < objects_number; ++i)
                {
                    ObjectData object_data;
                    data >> object_data;
                    addObject(object_data);
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


    const std::unordered_map<sf::Vector2f, std::vector<ObjectData>>& UdpManager::getObjectDataPool() const
    {
        return object_data_pool;
    }


    std::unordered_map<std::string, PlayerData>::iterator UdpManager::removePlayerById(const std::string& id)
    {
        auto iter = player_data_pool.find(id);
        if (iter == player_data_pool.end())
            return std::unordered_map<std::string, PlayerData>::iterator();
        else
            return player_data_pool.erase(iter);
    }


    void UdpManager::removeObject(const ObjectData& obj_data)
    {
        /* auto iter = object_data_pool.find(obj_data.getPosition() - sf::Vector2f(std::fmod(obj_data.getPosition().x, 16.f), std::fmod(obj_data.getPosition().y, 16.f)));
        if (iter == object_data_pool.end())
        {
            return std::unordered_map<sf::Vector2f, std::vector<ObjectData>>::iterator();
        }
        auto oiter = std::find(iter->second.begin(), iter->second.end(), obj_data);
        if (oiter == iter->second.end())
            return std::unordered_map<sf::Vector2f, std::vector<ObjectData>>::iterator();
        else
        {
            removed_object_data_list.push_back(*oiter);
            iter->second.erase(oiter);
            if (iter->second.empty())
                return object_data_pool.erase(iter);
            else
                return iter;
        } */
        removed_object_data_list.push_back(obj_data);
    }


    void UdpManager::addObject(const Object::Object& object)
    {
        ObjectData object_data( object.getPosition() / static_cast<float>(Constants::getPIXEL_SIZE()),
                                static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()),
                                object.getName(),
                                object.getPassability());
        addObject(object_data);
    }


    void UdpManager::addObject(const Multiplayer::ObjectData& object_data)
    {
        auto iter = object_data_pool.find(object_data.getPosition() - sf::Vector2f(std::fmod(object_data.getPosition().x, 16.f), std::fmod(object_data.getPosition().y, 16.f)));
        if (iter == object_data_pool.end())
        {
            std::vector<ObjectData> objects;
            objects.push_back(object_data);
            object_data_pool[object_data.getPosition() - sf::Vector2f(std::fmod(object_data.getPosition().x, 16.f), std::fmod(object_data.getPosition().y, 16.f))] = objects;
        }
        else
            iter->second.push_back(object_data);
    }


    ObjectData UdpManager::getRemovedObjectData()
    {
        if (removed_object_data_list.empty())
        {
            return ObjectData();
        }
        else
        {
            ObjectData& object_data = removed_object_data_list.back();
            removed_object_data_list.pop_back();
            return object_data;
        }
    }


    ObjectData UdpManager::getObjectToInventoryData()
    {
        if (objects_to_inventory_list.empty())
        {
            return ObjectData();
        }
        else
        {
            ObjectData& object_data = objects_to_inventory_list.back();
            objects_to_inventory_list.pop_back();
            return object_data;
        }
    }


    sf::Socket::Status UdpManager::send(sf::Packet& packet, const sf::IpAddress& dest_ip)
    {
        if (dest_ip == sf::IpAddress())
            return socket_send.send(packet, address_send, port);
        else
            return socket_send.send(packet, dest_ip, port); // ?
    }


    #ifndef CLIENT
        void UdpManager::addObjectByNoise(const sf::Vector2f& position)
        {
            sf::Vector2f point (position.x - std::fmod(position.x, 16.f), position.y - std::fmod(position.y, 16.f));
            if (!object_data_pool.count(point))
            {
                auto noise = perlin.octave2D_01((point.x * 0.01), (point.y * 0.01), 4);
                Object::ObjectName object_name;
                Object::Passability passability = Object::Passability::background;
                sf::Uint32 time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
                if (noise > .8)
                {
                    object_name = Object::ObjectName::stone;
                    passability = Object::Passability::impassible;
                }
                else if (noise > .5)
                {
                    object_name = Object::ObjectName::dirty_grass;
                }
                else
                {
                    object_name = Object::ObjectName::dirt;
                }
                addObject(Multiplayer::ObjectData(point, time_now, object_name, passability));
                //for (auto iter : player_data_pool)
                //{
                //    sf::Packet data;
                //    data << DataType::Object << object_data_pool.at(point);
                //    while (send(data, sf::IpAddress(iter.second.getLocalIp())) != sf::Socket::Status::Done);
                //}
            }
        }


        sf::Packet UdpManager::checkSector(const sf::Vector2f& position)
        {
            sf::Packet data;
            data << DataType::Sector;
            data << static_cast<sf::Uint32>(Constants::getVIEW_RADIUS() * 2 * Constants::getVIEW_RADIUS() * 2);
            sf::Vector2f point (position.x - std::fmod(position.x, 16.), position.y - std::fmod(position.y, 16.));
            for (auto y = -Constants::getVIEW_RADIUS(); y < Constants::getVIEW_RADIUS(); ++y)
            {
                for (auto x = -Constants::getVIEW_RADIUS(); x < Constants::getVIEW_RADIUS(); ++x)
                {
                    addObjectByNoise(point + sf::Vector2f(x * 16., y * 16.));
                    for (auto object_data : object_data_pool.at(point + sf::Vector2f(x * 16., y * 16.))) {
                        data << object_data;
                    }
                }
            }
            return data;
        }
    #endif
}