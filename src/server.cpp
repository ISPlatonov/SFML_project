#include <SFML/Network.hpp>
#include <SFML/System.hpp>
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


void UdpWorker(Multiplayer::UdpManager& UdpManager)
{
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
}


int main()
{
    std::cout << std::flush << std::endl;
    Multiplayer::UdpManager UdpManager(sf::IpAddress::getLocalAddress(), sf::IpAddress::Any);

    while (true)
    {
        // needs another thread
        auto last_timepoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        
        UdpWorker(UdpManager);
 
        //while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - last_timepoint < 10);
        //std::cout << std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - last_timepoint) << std::endl;
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - last_timepoint;
        sf::sleep(((1000 / FRAMERATE_LIMIT) >= dt ? sf::milliseconds((1000 / FRAMERATE_LIMIT) - dt) : sf::Time()));
    }
    return EXIT_SUCCESS;
}
