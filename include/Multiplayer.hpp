#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <chrono>
//#include "Actor.hpp"
#include "Constants.hpp"


namespace Multiplayer
{
    class PlayerData
    {
    public:
        PlayerData();
        PlayerData(const PlayerData& player);
        PlayerData(sf::Vector2f position, int ip, int local_ip, sf::Uint32 time);
        void setPosition(const sf::Vector2f& new_position);
        void setTime(const sf::Uint32& new_time);
        const sf::Vector2f& getPosition() const;
        const int& getIp() const;
        const int& getLocalIp() const;
        const sf::Uint32& getTime() const;

    private:
        // data >> new_position.x >> new_position.y >> msg_ip >> msg_local_ip >> sent_time;
        sf::Vector2f position;
        int ip, local_ip;
        sf::Uint32 sent_time;
    };


    class UdpManager
    {
    public:
        UdpManager(const sf::IpAddress& address_receive, const sf::IpAddress& address_send);
        sf::Socket::Status receive();
        void send(sf::Packet& packet, const sf::IpAddress& dest_ip = sf::IpAddress());
        const std::map<std::string, PlayerData>& getPlayerDataPool() const;
        std::map<std::string, PlayerData>::iterator removePlayerById(const std::string& id);

    private:
        sf::UdpSocket socket;
        sf::UdpSocket socket_send;
        sf::IpAddress address_send;
        sf::IpAddress address_receive;
        sf::IpAddress ip, local_ip;
        unsigned short port;
        unsigned short port_send;
        std::map<std::string, PlayerData> player_data_pool;
    };
}