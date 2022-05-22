//#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <map>
#include <string>
#include <cstdlib>
//#include "LinearAlgebra.hpp"
//#include "Multiplayer.hpp"
#include <Constants.hpp>
#include <chrono>
#include <vector>
#include <set>
#include <iostream>


class PlayerData
{
public:
    PlayerData()
    {
        position = sf::Vector2f();
        ip = sf::IpAddress::None.toInteger();
        local_ip = sf::IpAddress::None.toInteger();
        sent_time = 0;
    }
    PlayerData(const PlayerData& player)
    {
        this->position = player.getPosition();
        this->ip = player.getIp();
        this->local_ip = player.getLocalIp();
        this->sent_time = player.getTime();
    }
    PlayerData(sf::Vector2f position, int ip, int local_ip, sf::Uint32 time)
    {
        this->position = position;
        this->ip = ip;
        this->local_ip = local_ip;
        this->sent_time = time;
    }
    void updatePosition(const sf::Vector2f& new_position)
    {
        position = new_position;
    }
    const sf::Vector2f getPosition() const
    {
        return position;
    }
    const int getIp() const
    {
        return ip;
    }
    const int getLocalIp() const
    {
        return local_ip;
    }
    const sf::Uint32 getTime() const
    {
        return sent_time;
    }

private:
    // data >> new_position.x >> new_position.y >> msg_ip >> msg_local_ip >> sent_time;
    sf::Vector2f position;
    int ip, local_ip;
    sf::Uint32 sent_time;
};


int main()
{
    std::cout << std::flush << std::endl;
    sf::UdpSocket socket;
    sf::UdpSocket socket_send;
    unsigned short port = PORT_LISTEN;
    unsigned short port_send = PORT_SEND;
    auto broadcast_ip = sf::IpAddress::Any;
    auto my_ip = sf::IpAddress::getPublicAddress();
    auto my_local_ip = sf::IpAddress::getLocalAddress();
    sf::IpAddress address_send(broadcast_ip);
    socket.setBlocking(false);
    sf::IpAddress address_receive(my_local_ip);
    // bind the socket to a port
    if (socket.bind(port) != sf::Socket::Done)
        return EXIT_FAILURE;
    if (socket_send.bind(port_send) != sf::Socket::Done)
        return EXIT_FAILURE;

    // Start the game loop
    sf::Packet data;
    sf::Vector2f multiplayer_position;
    // int ip, Actor::Player player
    std::map<std::string, PlayerData> player_pool;
    std::set<std::string> ip_pool;

    while (true)
    {
        // needs another thread
        //Mob::multiplayers_list.clear();
        for (size_t i = 0; i < ip_pool.size() + 10; ++i)
        {
            //data.clear();
            int msg_local_ip, msg_ip;
            sf::Uint32 sent_time;
            sf::Vector2f new_position;
            auto status = socket.receive(data, address_receive, port_send);
            if (status != sf::Socket::Status::Done)
                continue;
	    
            data >> new_position.x >> new_position.y >> msg_ip >> msg_local_ip >> sent_time;
	    if (msg_ip == broadcast_ip.toInteger() && msg_local_ip == broadcast_ip.toInteger())
                continue;
            if (msg_ip == my_ip.toInteger() && msg_local_ip == my_local_ip.toInteger())
                continue;

            auto id = sf::IpAddress(msg_ip).toString() + ' ' + sf::IpAddress(msg_local_ip).toString();
	    std::cout << "got new ip: " << id << std::endl;

            sf::Uint32 time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            sf::Uint32 ping = time_now - sent_time;

	    std::cout << "new msg" << std::endl << "ping: " << std::to_string(ping) << std::endl;

            if (player_pool.count(id))
	    {
		std::cout << "player in pool" << std::endl;
                if (ping > MAX_PING)
                {
                    player_pool.erase(id);
                    ip_pool.erase(id);
                }
                else
                    player_pool[id].updatePosition(new_position);
	    }
            else
                if (ping > MAX_PING)
                    continue;
                else
                {
                    player_pool[id] = PlayerData(new_position, msg_ip, msg_local_ip, sent_time);
                    ip_pool.insert(id);
                }
        }

        for (auto ip : ip_pool)
        {
            data.clear();
            data << player_pool[ip].getPosition().x << player_pool[ip].getPosition().y << player_pool[ip].getIp() << player_pool[ip].getLocalIp() << player_pool[ip].getTime();
	        //std::cout << "dest. address: " << sf::IpAddress(player_pool[ip].getIp()).toString() << ' ' << sf::IpAddress(player_pool[ip].getLocalIp()).toString() << std::endl;
            socket_send.send(data, sf::IpAddress(player_pool[ip].getLocalIp()), port);
        }      
        
        data.clear();
    }
    return EXIT_SUCCESS;
}
