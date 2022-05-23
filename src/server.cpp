//#include <SFML/Audio.hpp>
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
    std::map<std::string, Multiplayer::PlayerData> player_pool;
    std::set<std::string> ip_pool;

    while (true)
    {
        // needs another thread
        auto last_timepoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

        for (size_t i = 0; i < ip_pool.size() + 1000; ++i)
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

            sf::Uint32 time_now = static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
            int ping = static_cast<int>(time_now) - static_cast<int>(sent_time);

	        std::cout << "new msg" << std::endl << "ping: " << std::to_string(ping) << std::endl
                << "server time: " << std::to_string(time_now) << std::endl
                << "msg time: " << std::to_string(sent_time) << std::endl
                << "ping: " << std::to_string(ping) << std::endl;

            if (player_pool.count(id))
            {
                std::cout << "player in pool" << std::endl;
                if (ping > MAX_PING)
                {
                    player_pool.erase(id);
                    ip_pool.erase(id);
                }
                else
                {
                    player_pool[id].updatePosition(new_position);
                    player_pool[id].updateTime(sent_time);
                }
            }
            else
                if (ping > MAX_PING)
                    continue;
                else
                {
                    player_pool[id] = Multiplayer::PlayerData(new_position, msg_ip, msg_local_ip, sent_time);
                    ip_pool.insert(id);
                }
            }

        // NEED TO FIX!!!
        /*for (auto id : ip_pool)
        {
            for (auto dest_ip : ip_pool)
            {
                if (dest_ip == id)
                    continue;
                data.clear();
                auto x = player_pool[id].getPosition().x;
                auto y = player_pool[id].getPosition().y;
                auto ip = player_pool[id].getIp();
                auto local_ip = player_pool[id].getLocalIp();
                auto time = player_pool[id].getTime();
                sf::Uint32 time_now = static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
                int ping = static_cast<int>(time_now) - static_cast<int>(time);
                if (!ip_pool.count(id))
                    continue;
                if (ping > MAX_PING)
                {
                    player_pool.erase(id);
                    ip_pool.erase(id);
                }
                data << x << y << ip << local_ip << time;
                //std::cout << "dest. address: " << sf::IpAddress(player_pool[ip].getIp()).toString() << ' ' << sf::IpAddress(player_pool[ip].getLocalIp()).toString() << std::endl;
                socket_send.send(data, sf::IpAddress(player_pool[dest_ip].getLocalIp()), port);
            }
        } */

        for (auto iter = ip_pool.begin(); iter != ip_pool.end();)
        {
            auto x = player_pool[*iter].getPosition().x;
            auto y = player_pool[*iter].getPosition().y;
            auto ip = player_pool[*iter].getIp();
            auto local_ip = player_pool[*iter].getLocalIp();
            auto time = player_pool[*iter].getTime();
            sf::Uint32 time_now = static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
            int ping = static_cast<int>(time_now) - static_cast<int>(time);
            //if (!ip_pool.count(*iter))
            //    continue;
            if (ping > MAX_PING)
            {
                player_pool.erase(*iter);
                ip_pool.erase(*iter++);
                continue;
            }
            for (auto dest_iter = ip_pool.begin(); iter != ip_pool.end();)
            {
                if (*dest_iter == *iter)
                {
                    ++iter;
                    break;
                }
                data.clear();
                data << x << y << ip << local_ip << time;
                socket_send.send(data, sf::IpAddress(player_pool[*dest_iter].getLocalIp()), port);
            }
        }     
        
        data.clear();

        while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - last_timepoint < 1);
    }
    return EXIT_SUCCESS;
}
