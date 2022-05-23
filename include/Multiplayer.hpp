#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <chrono>
#include "Actor.hpp"


namespace Multiplayer
{
    class PlayerData
    {
    public:
        PlayerData();
        PlayerData(const PlayerData& player);
        PlayerData(sf::Vector2f position, int ip, int local_ip, sf::Uint32 time);
        void updatePosition(const sf::Vector2f& new_position);
        void updateTime(const sf::Uint32& new_time);
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
}