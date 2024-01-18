#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include "Constants.hpp"
#include "Object.hpp"
#include "Hash.hpp"
#include "PerlinNoise.hpp"
#include <string>
#include <chrono>
#include <unordered_map>
#include <vector>


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
        Transportable() {}
        Transportable(sf::Vector2f pos, Time::Time sent) : position(pos), sent_time(sent) {}
        inline void setPosition(const sf::Vector2f& new_position) { position = new_position; };
        inline void setTime(const Time::Time& new_time) { sent_time = new_time; }
        inline const sf::Vector2f& getPosition() const { return position; }
        inline const Time::Time& getTime() const { return sent_time; }

    private:
        sf::Vector2f position;
        Time::Time sent_time = 0;
    };


    typedef std::unordered_map<Object::ObjectName, size_t> Inventory;

    /**
     * @brief Class for trasferral player data
     * @details Player data is a data about player's position, inventory and time of sending
    */
    class PlayerData : virtual public Transportable
    {
    public:
        PlayerData() : Transportable::Transportable() {}
        PlayerData(const sf::Vector2f& p, const PlayerId& p_id, const Time::Time& t, const Inventory& ni)
            : Transportable::Transportable(p, t), player_id(p_id), inventory(ni) {}
        PlayerData(const PlayerData& player)
            : PlayerData(player.getPosition(), player.getId(), player.getTime(), player.getInventory()) {}
        inline PlayerId getId() const { return player_id; }
        const size_t objectNumber(Object::ObjectName) const;
        const size_t addObject(Object::ObjectName);
        const size_t removeObject(Object::ObjectName);
        const Inventory& getInventory() const { return inventory; }

    private:
        // data >> new_position.x >> new_position.y >> msg_ip >> port >> sent_time;
        PlayerId player_id;
        Inventory inventory;
    };


    /**
     * @brief Class for trasferral object data
     * @details Object data is a data about object's position, type and time of sending
    */
    class ObjectData : virtual public Transportable
    {
    public:
        ObjectData() : Transportable::Transportable() {}
        ObjectData(sf::Vector2f pos, Time::Time t, Object::ObjectName name, Object::Passability pass)
            : Transportable::Transportable(pos, t), object_name(name), passability(pass) {}
        ObjectData(const ObjectData& object) : ObjectData(object.getPosition(), object.getTime(), object.getName(), object.getPassability()) {}
        const Object::ObjectName& getName() const { return object_name; }
        const Object::Passability& getPassability() const { return passability; }
        inline bool operator ==(const ObjectData& object_data) const { return object_name == object_data.getName() && passability == object_data.getPassability(); }

    private:
        Object::ObjectName object_name;
        Object::Passability passability;
    };


    typedef std::unordered_map<SocketInfo, PlayerData> PlayerDataPool;
    typedef std::unordered_map<sf::Vector2f, std::vector<ObjectData>> ObjectDataPool;

    /**
     * @brief Class for managing multiplayer
    */
    class UdpManager
    {
    public:
        UdpManager(const sf::IpAddress& address_receive, const sf::IpAddress& address_send);
        sf::Socket::Status receive();
        sf::Socket::Status send(sf::Packet& packet, const sf::IpAddress& dest_ip = sf::IpAddress(), const unsigned short& dest_port = 0);
        inline const PlayerDataPool& getPlayerDataPool() const { return player_data_pool; }
        inline const ObjectDataPool& getObjectDataPool() const { return object_data_pool; }
        inline void clearObjectDataPool() { object_data_pool.clear(); }
        PlayerDataPool::iterator removePlayerBySocketInfo(const SocketInfo& id);
        bool removeObject(const ObjectData&);
        void addObject(const Object::Object& object);
        void addObject(const Multiplayer::ObjectData& object_data);
        ObjectData getRemovedObjectData();
        ObjectData getObjectToInventoryData();
        sf::Packet checkSector(const sf::Vector2f&);
        inline const unsigned int getLocalPort() const { return port; };

    private:
        sf::UdpSocket socket;
        /*static*/ sf::IpAddress ip;
        /*static*/ unsigned short port;
        /*static*/ PlayerDataPool player_data_pool;
        /*static*/ ObjectDataPool object_data_pool;
        /*static*/ /* std::unordered_map<sf::Vector2f, ObjectData> terrain_data_pool; */
        /*static*/ std::vector<ObjectData> removed_object_data_list;
        /*static*/ std::vector<ObjectData> objects_to_inventory_list;
        /*static*/ sf::Packet data;
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