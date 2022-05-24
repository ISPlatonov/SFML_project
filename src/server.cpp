#include <SFML/Network.hpp>
#include <map>
#include <string>
#include <cstdlib>
//#include "LinearAlgebra.hpp"
#include "Multiplayer.hpp"
#include <Constants.hpp>
#include <chrono>
#include <vector>
#include <set>
#include <iostream>


int main()
{
    std::cout << std::flush << std::endl;
    /*sf::UdpSocket socket;
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
        return EXIT_FAILURE;*/

    //sf::Packet data;
    //sf::Vector2f multiplayer_position;
    //std::map<std::string, Multiplayer::PlayerData> player_pool;
    //std::set<std::string> ip_pool;
    Multiplayer::UdpManager UdpManager(sf::IpAddress::getLocalAddress(), sf::IpAddress::Any);

    while (true)
    {
        // needs another thread
        //auto last_timepoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        

        for (size_t i = 0; i < UdpManager.getPlayerDataPool().size() + UDP_PACKETS_GAP; ++i)
        {
            UdpManager.receive();
        }

        for (auto iter = UdpManager.getPlayerDataPool().begin(); iter != UdpManager.getPlayerDataPool().end();)
        {
            std::cout << "unpacking player from pool: " << (*iter).first << std::endl;
            auto x = (*iter).second.getPosition().x;
            auto y = (*iter).second.getPosition().y;
            auto ip = (*iter).second.getIp();
            auto local_ip = (*iter).second.getLocalIp();
            auto time = (*iter).second.getTime();
            sf::Uint32 time_now = static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
            int ping = static_cast<int>(time_now) - static_cast<int>(time);
            std::cout << "id: " << (*iter).first << ", last timepoint: " << std::to_string(time) << std::endl;

            if (ping > MAX_PING)
            {
                std::cout << "reached MAX_PING" << std::endl;
                UdpManager.removePlayerById((*iter++).first);
                continue;
            }
            for (auto dest_iter = UdpManager.getPlayerDataPool().begin(); dest_iter != UdpManager.getPlayerDataPool().end();)
            {
                std::cout << "sending " << (*iter).first << " data to " << (*dest_iter).first << std::endl;
                if ((*dest_iter).first == (*iter).first)
                {
                    std::cout << "*dest_iter == *iter" << std::endl;
                    ++dest_iter;
                    std::cout << "made ++dest_iter" << std::endl;
                    continue;
                }
                sf::Packet data;
                data << x << y << ip << local_ip << time;
                UdpManager.send(data, sf::IpAddress((*dest_iter++).second.getLocalIp()));
                std::cout << "sent" << std::endl;
            }
            ++iter;
        }     
        //while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - last_timepoint < 1);
    }
    return EXIT_SUCCESS;
}
