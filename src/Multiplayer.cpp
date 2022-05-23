#include "Multiplayer.hpp"


namespace Multiplayer
{
    PlayerData::PlayerData()
    {
        position = sf::Vector2f();
        ip = sf::IpAddress::None.toInteger();
        local_ip = sf::IpAddress::None.toInteger();
        sent_time = 0;
    }
    
    
    PlayerData::PlayerData(const PlayerData& player)
    {
        this->position = player.getPosition();
        this->ip = player.getIp();
        this->local_ip = player.getLocalIp();
        this->sent_time = player.getTime();
    }
        
        
    PlayerData::PlayerData(sf::Vector2f position, int ip, int local_ip, sf::Uint32 time)
    {
        this->position = position;
        this->ip = ip;
        this->local_ip = local_ip;
        this->sent_time = time;
    }
    
    
    void PlayerData::updatePosition(const sf::Vector2f& new_position)
    {
        position = new_position;
    }
    
    
    void PlayerData::updateTime(const sf::Uint32& new_time)
    {
        sent_time = new_time;
    }
        
        
    const::sf::Vector2f& PlayerData::getPosition() const
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
    
    
    const sf::Uint32& PlayerData::getTime() const
    {
        return sent_time;
    }
}