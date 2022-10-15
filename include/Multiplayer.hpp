#pragma once

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <chrono>
#include <unordered_map>
//#include "Actor.hpp"
#include "Constants.hpp"
#include "Object.hpp"
#include "Hash.hpp"


namespace Multiplayer
{
    enum DataType
    {
        Player,
        Object
    };


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


    class ObjectData : virtual public Transportable
    {
    public:
        ObjectData();
        ObjectData(const ObjectData& object);
        ObjectData(sf::Vector2f position, sf::Uint32 time, Object::ObjectName object_name, Object::Passability passability);
        const Object::ObjectName& getName() const;
        const Object::Passability& getPassability() const;

    private:
        Object::ObjectName object_name;
        Object::Passability passability;
    };


    class UdpManager
    {
    public:
        UdpManager(const sf::IpAddress& address_receive, const sf::IpAddress& address_send);
        sf::Socket::Status receive();
        void send(sf::Packet& packet, const sf::IpAddress& dest_ip = sf::IpAddress());
        const std::unordered_map<std::string, PlayerData>& getPlayerDataPool() const;
        const std::unordered_map<sf::Vector2f, ObjectData>& getObjectDataPool() const;
        std::unordered_map<std::string, PlayerData>::iterator removePlayerById(const std::string& id);
        std::unordered_map<sf::Vector2f, ObjectData>::iterator removeObjectByPoint(const sf::Vector2f& point);
        void addObject(const Object::Object& object);
        void addObject(const Multiplayer::ObjectData& object_data);

    private:
        sf::UdpSocket socket;
        sf::UdpSocket socket_send;
        /*static*/ sf::IpAddress address_send;
        /*static*/ sf::IpAddress address_receive;
        /*static*/ sf::IpAddress ip, local_ip;
        /*static*/ unsigned short port;
        /*static*/ unsigned short port_send;
        /*static*/ std::unordered_map<std::string, PlayerData> player_data_pool;
        /*static*/ std::unordered_map<sf::Vector2f, ObjectData> object_data_pool;
    };


    sf::Packet& operator <<(sf::Packet& packet, const Object::Object& object);
    Object::Object& operator <<(Object::Object& object, const ObjectData& object_data);
}