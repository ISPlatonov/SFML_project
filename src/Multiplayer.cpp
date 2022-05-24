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
    
    
    void PlayerData::setPosition(const sf::Vector2f& new_position)
    {
        position = new_position;
    }
    
    
    void PlayerData::setTime(const sf::Uint32& new_time)
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
        //data.clear();
        sf::Packet data;
        int msg_local_ip, msg_ip;
        sf::Uint32 sent_time;
        sf::Vector2f new_position;
        auto status = socket.receive(data, address_receive, port_send);
        if (status != sf::Socket::Status::Done)
            return status;
        data >> new_position.x >> new_position.y >> msg_ip >> msg_local_ip >> sent_time;
        //if (msg_ip == my_ip.toInteger() && msg_local_ip == my_local_ip.toInteger())
        //    continue;
        
        auto id = sf::IpAddress(msg_ip).toString() + sf::IpAddress(msg_local_ip).toString();

        sf::Uint32 time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        int ping = static_cast<int>(time_now) - static_cast<int>(sent_time);
        if (player_data_pool.count(id))
            if (ping > MAX_PING)
            {
                player_data_pool.erase(id);
            }
            else
            {
                player_data_pool[id].setPosition(new_position);
                player_data_pool[id].setTime(sent_time);
            }
        else
            if (ping > MAX_PING)
                return status;
            else
            {
                player_data_pool[id] = PlayerData(new_position, msg_ip, msg_local_ip, sent_time);
            }
        return status;
    }


    const std::map<std::string, PlayerData>& UdpManager::getPlayerDataPool() const
    {
        return player_data_pool;
    }


    std::map<std::string, PlayerData>::iterator UdpManager::removePlayerById(const std::string& id)
    {
        if (!player_data_pool.count(id))
            return std::map<std::string, PlayerData>::iterator();
        else
            return player_data_pool.erase(player_data_pool.find(id));
    }


    void UdpManager::send(sf::Packet& packet, const sf::IpAddress& dest_ip)
    {
        if (dest_ip == sf::IpAddress())
            socket_send.send(packet, address_send, port);
        socket_send.send(packet, dest_ip, port);
    }
}