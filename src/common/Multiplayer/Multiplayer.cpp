#include "Multiplayer.hpp"
#include <iostream>


namespace Multiplayer
{
    const size_t PlayerData::addObject(Object::ObjectName name)
    {
        inventory.count(name) ? inventory[name] += 1 : inventory[name] = 1;
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


    UdpManager::UdpManager(const sf::IpAddress& address_receive, const sf::IpAddress& address_send)
    {
        #ifdef CLIENT
            port = sf::Socket::AnyPort; // sf::Socket::AnyPort;
        #else
            port = Constants::getPORT_LISTEN();
        #endif

        ip = sf::IpAddress::getPublicAddress(sf::seconds(Constants::getMAX_PING()));
        socket.setBlocking(false);
        // bind the socket to a port
        while (socket.bind(port) != sf::Socket::Status::Done);
        port = socket.getLocalPort();
        std::cout << "ip: " << sf::IpAddress::getLocalAddress() << std::endl;
        std::cout << "port: " << port << std::endl;
    }


    sf::Socket::Status UdpManager::receive()
    {
        data.clear();
        SocketInfo socket_info;
        auto status = sf::Socket::Status::NotReady;
        status = socket.receive(data, socket_info.first, socket_info.second);
        if (status != sf::Socket::Status::Done)
            return status;
        DataType data_type;
        {
            sf::Uint32 data_type_enum;
            data >> data_type_enum;
            data_type = static_cast<DataType>(data_type_enum);
        }

        switch (data_type)
        {
            case DataType::Object:
            {
                // receive object
                Multiplayer::ObjectData object_data;
                data >> object_data;
                object_data.setSocketInfo(socket_info);
                addObject(object_data);
                break;
            }
            case DataType::Player:
            {
                PlayerData player_data;
                data >> player_data;
                player_data.setSocketInfo(socket_info);
                auto id = player_data.getId();
                Time::Time time_now = Time::getTimeNow();
                int ping = static_cast<int>(time_now) - static_cast<int>(player_data.getTime());
                if (player_data_pool.count(id))
                {
                    #ifndef CLIENT
                        auto old_time = player_data_pool[id].getTime();
                        int old_ping = static_cast<int>(time_now) - static_cast<int>(old_time);
                        if (old_ping > Constants::getMAX_PING())
                        {
                            // send foreground objects on map
                            player_data_pool[id].setSocketInfo(socket_info);
                            for (auto iter : object_data_pool)
                            {
                                for (auto obj_data : iter.second)
                                {
                                    if (obj_data.getPassability() != Object::Passability::foreground)
                                        continue;
                                    data.clear();
                                    data << DataType::Object << obj_data;
                                    send(data, socket_info.first, socket_info.second);
                                    sf::sleep(sf::milliseconds(1));
                                }
                            }
                            if (player_data.getInventory().empty() && !player_data_pool[id].getInventory().empty())
                            {
                                // send all inventory
                                for (const auto& iter : player_data_pool[id].getInventory())
                                {
                                    for (size_t i = 0; i < iter.second; ++i)
                                    {
                                        data.clear();
                                        auto object_data = ObjectData(sf::Vector2f(0, 0), old_time, socket_info, iter.first, Object::Passability::foreground);
                                        data << DataType::Event << EventType::addObjectToInvectory << object_data;
                                        send(data, socket_info.first, socket_info.second);
                                    }
                                }
                            }
                            else
                                for (const auto& iter : player_data_pool[id].getInventory())
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
                        /* for (auto iter : object_data_pool)
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
                        } */
                    }
                break;
            }
            case DataType::Event:
            {
                EventType event_type;
                // event handling
                {
                    sf::Uint32 event_type_enum;
                    data >> event_type_enum;
                    event_type = static_cast<EventType>(event_type_enum);
                }
                switch (event_type)
                {
                    #ifndef CLIENT // SERVER
                    case EventType::takeObjectToInventory:
                    {
                        // data >> object_data >> user
                        // receive object
                        ObjectData object_data;
                        data >> object_data;
                        // receive user
                        PlayerData player_data;
                        data >> player_data;
                        auto id = player_data.getId();
                        Time::Time time_now = Time::getTimeNow();
                        int ping = static_cast<int>(time_now) - static_cast<int>(player_data.getTime());
                        if (player_data_pool.count(id))
                            if (ping > Constants::getMAX_PING())
                            {
                                //player_data_pool.erase(id);
                            }
                            else
                            {
                                auto& player = player_data_pool[id];
                                player.setPosition(player_data.getPosition());
                                player.setTime(player_data.getTime());
                                player.addObject(object_data.getName());
                                data.clear();
                                if (!removeObject(object_data)) {
                                    // no such object in the world :|
                                    break;
                                }
                                data << DataType::Event << EventType::removeObject << object_data;
                                for (const auto& iter : getPlayerDataPool())
                                    send(data, iter.second.getSocketInfo().first, iter.second.getSocketInfo().second);
                                data.clear();
                                data << DataType::Event << EventType::addObjectToInvectory << object_data;
                                send(data, socket_info.first, socket_info.second);
                            }
                        break;
                    }
                    #endif

                    #ifdef CLIENT // CLIENT
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
                    #endif

                    default:
                        break;
                }
                break;
            }
            case DataType::Sector:
            {
                // data >> objects >> ...
                while (!data.endOfPacket())
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


    PlayerDataPool::iterator UdpManager::removePlayerByPlayerId(const PlayerId& id)
    {
        auto iter = player_data_pool.find(id);
        if (iter == player_data_pool.end())
            return PlayerDataPool::iterator();
        else
            return player_data_pool.erase(iter);
    }


    /**
     * @brief removes object from object_data_pool
     * 
     * @param obj_data: object to remove
     * @return true if object was removed, false if there was no such object
    */
    bool UdpManager::removeObject(const ObjectData& obj_data)
    {
        removed_object_data_list.push_back(obj_data);
        auto iter = object_data_pool.find(obj_data.getPosition() - sf::Vector2f(std::fmod(obj_data.getPosition().x, 16.f), std::fmod(obj_data.getPosition().y, 16.f)));
        if (iter == object_data_pool.end())
            return false;

        auto oiter = std::find(iter->second.begin(), iter->second.end(), obj_data);
        if (oiter == iter->second.end())
            return false;
        else
        {
            iter->second.erase(oiter);
            if (iter->second.empty()) {
                object_data_pool.erase(iter);
            }
            return true;
        }
    }


    void UdpManager::addObject(const Object::Object& object)
    {
        ObjectData object_data( object.getPosition() / static_cast<float>(Constants::getPIXEL_SIZE()),
                                Time::getTimeNow(),
                                SocketInfo(0, 0), // ?
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


    sf::Socket::Status UdpManager::send(sf::Packet& packet, const sf::IpAddress& dest_ip, const unsigned short& dest_port)
    {
        if (dest_ip == sf::IpAddress())
            return socket.send(packet, Constants::getSERVER_IP(), Constants::getPORT_LISTEN());
        else
            return socket.send(packet, dest_ip, dest_port); // ?
    }


    #ifndef CLIENT
        void UdpManager::addObjectByNoise(const sf::Vector2f& position)
        {
            sf::Vector2f point (position.x - std::fmod(position.x, 16.f), position.y - std::fmod(position.y, 16.f));
            auto iter = object_data_pool.find(point);
            if (iter == object_data_pool.end() || find_if(iter->second.begin(), iter->second.end(), [](const ObjectData& obj_data){ const auto& pass = obj_data.getPassability(); return pass == Object::Passability::background || pass == Object::Passability::impassible; }) == iter->second.end())
            {
                auto noise = perlin.octave2D_01((point.x * 0.01), (point.y * 0.01), 4);
                Object::ObjectName object_name;
                Object::Passability passability = Object::Passability::background;
                Time::Time time_now = Time::getTimeNow();
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
                addObject(Multiplayer::ObjectData(point, time_now, SocketInfo(0, 0), object_name, passability));
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
            //data << static_cast<sf::Uint32>(Constants::getVIEW_RADIUS() * 2 * Constants::getVIEW_RADIUS() * 2);
            sf::Vector2f point (position.x - std::fmod(position.x, 16.), position.y - std::fmod(position.y, 16.));
            for (auto y = -Constants::getVIEW_RADIUS(); y < Constants::getVIEW_RADIUS(); ++y)
            {
                for (auto x = -Constants::getVIEW_RADIUS(); x < Constants::getVIEW_RADIUS(); ++x)
                {
                    addObjectByNoise(point + sf::Vector2f(x * 16., y * 16.));
                    auto& vec = object_data_pool[point + sf::Vector2f(x * 16., y * 16.)];
                    for (auto object_data : vec) {
                        data << object_data;
                    }
                }
            }
            return data;
        }
    #endif
}