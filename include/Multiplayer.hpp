#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
//#include "Actor.hpp"
#include "Constants.hpp"
#include "Object.hpp"
#include "Hash.hpp"
#include "PerlinNoise.hpp"
#include <string>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <cmath>


namespace Multiplayer
{
    enum DataType
    {
        Player,
        Object,
        Event,
        Sector
    };


    enum EventType
    {
        takeObjectToInventory,
        ejectObjectFromInventory,
        removeObject,
        addObjectToInvectory
    };


    /**
     * @brief Class for trasferral data between server and client
    */
    class Transportable
    {
    public:
        Transportable();
        Transportable(sf::Vector2f position, sf::Uint32 sent_time);
        void setPosition(const sf::Vector2f& new_position);
        void setTime(const sf::Uint32& new_time);
        const sf::Vector2f& getPosition() const;
        const sf::Uint32& getTime() const;

    private:
        sf::Vector2f position;
        sf::Uint32 sent_time;
    };


    /**
     * @brief Class for trasferral player data
     * @details Player data is a data about player's position, inventory and time of sending
    */
    class PlayerData : virtual public Transportable
    {
    public:
        PlayerData();
        PlayerData(const PlayerData& player);
        PlayerData(const sf::Vector2f& position, const int& ip, const int& local_ip, const sf::Uint32& time, const std::unordered_map<Object::ObjectName, size_t>& inventory);
        const int& getIp() const;
        const int& getLocalIp() const;
        const size_t objectNumber(Object::ObjectName) const;
        const size_t addObject(Object::ObjectName);
        const size_t removeObject(Object::ObjectName);
        const std::unordered_map<Object::ObjectName, size_t>& getInventory() const;

    private:
        // data >> new_position.x >> new_position.y >> msg_ip >> msg_local_ip >> sent_time;
        int ip, local_ip;
        std::unordered_map<Object::ObjectName, size_t> inventory;
    };


    /**
     * @brief Class for trasferral object data
     * @details Object data is a data about object's position, type and time of sending
    */
    class ObjectData : virtual public Transportable
    {
    public:
        ObjectData();
        ObjectData(const ObjectData& object);
        ObjectData(sf::Vector2f position, sf::Uint32 time, Object::ObjectName object_name, Object::Passability passability);
        const Object::ObjectName& getName() const;
        const Object::Passability& getPassability() const;
        inline bool operator ==(const ObjectData& object_data) const { return object_name == object_data.getName() && passability == object_data.getPassability(); }

    private:
        Object::ObjectName object_name;
        Object::Passability passability;
    };


    /**
     * @brief Class for managing multiplayer
    */
    class UdpManager
    {
    public:
        UdpManager(const sf::IpAddress& address_receive, const sf::IpAddress& address_send);
        sf::Socket::Status receive();
        sf::Socket::Status send(sf::Packet& packet, const sf::IpAddress& dest_ip = sf::IpAddress());
        const std::unordered_map<std::string, PlayerData>& getPlayerDataPool() const;
        const std::unordered_map<sf::Vector2f, std::vector<ObjectData>>& getObjectDataPool() const;
        inline void clearObjectDataPool() { object_data_pool.clear();}
        std::unordered_map<std::string, PlayerData>::iterator removePlayerById(const std::string& id);
        bool removeObject(const ObjectData&);
        void addObject(const Object::Object& object);
        void addObject(const Multiplayer::ObjectData& object_data);
        ObjectData getRemovedObjectData();
        ObjectData getObjectToInventoryData();
        sf::Packet checkSector(const sf::Vector2f&);

    private:
        sf::UdpSocket socket;
        sf::UdpSocket socket_send;
        /*static*/ sf::IpAddress address_send;
        /*static*/ sf::IpAddress address_receive;
        /*static*/ sf::IpAddress ip, local_ip;
        /*static*/ unsigned short port;
        /*static*/ unsigned short port_send;
        /*static*/ std::unordered_map<std::string, PlayerData> player_data_pool;
        /*static*/ std::unordered_map<sf::Vector2f, std::vector<ObjectData>> object_data_pool;
        /*static*/ /* std::unordered_map<sf::Vector2f, ObjectData> terrain_data_pool; */
        /*static*/ std::vector<ObjectData> removed_object_data_list;
        /*static*/ std::vector<ObjectData> objects_to_inventory_list;
        #ifndef CLIENT
            static inline const siv::PerlinNoise perlin{ 8u };
            void addObjectByNoise(const sf::Vector2f&);
        #endif
    };
}


sf::Packet& operator <<(sf::Packet& packet, Multiplayer::ObjectData& object_data);
Object::Object& operator <<(Object::Object& object, const Multiplayer::ObjectData& object_data);
sf::Packet& operator >>(sf::Packet&, Multiplayer::ObjectData&);
sf::Packet& operator <<(sf::Packet& packet, const Multiplayer::PlayerData& player_data);
sf::Packet& operator >>(sf::Packet&, Multiplayer::PlayerData&);