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
//#include <iostream>
#include <fstream>


void UdpWorker(Multiplayer::UdpManager& UdpManager)
{
    for (size_t i = 0; i < UdpManager.getPlayerDataPool().size() + Constants::getUDP_PACKETS_GAP(); ++i)
    {
        UdpManager.receive();
    }

    for (auto iter = UdpManager.getPlayerDataPool().begin(); iter != UdpManager.getPlayerDataPool().end(); ++iter)
    {
        //std::cout << "unpacking player from pool: " << (*iter).first << std::endl;
        auto x = (*iter).second.getPosition().x;
        auto y = (*iter).second.getPosition().y;
        auto ip = (*iter).second.getIp();
        auto local_ip = (*iter).second.getLocalIp();
        auto time = (*iter).second.getTime();
        sf::Uint32 time_now = static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
        int ping = static_cast<int>(time_now) - static_cast<int>(time);
        //std::cout << "id: " << (*iter).first << ", last timepoint: " << std::to_string(time) << std::endl;

        if (ping > Constants::getMAX_PING())
        {
            //std::cout << "reached MAX_PING" << std::endl;
            //UdpManager.removePlayerById((*iter++).first);
            continue;
        }
        for (auto dest_iter = UdpManager.getPlayerDataPool().begin(); dest_iter != UdpManager.getPlayerDataPool().end();)
        {
            //std::cout << "sending " << (*iter).first << " data to " << (*dest_iter).first << std::endl;
            if ((*dest_iter).first == (*iter).first)
            {
                //std::cout << "*dest_iter == *iter" << std::endl;
                ++dest_iter;
                //std::cout << "made ++dest_iter" << std::endl;
                continue;
            }
            sf::Packet data;
            data << static_cast<sf::Uint32>(Multiplayer::DataType::Player) << x << y << ip << local_ip << time;
            data << static_cast<sf::Uint32>((*iter).second.getInventory().size());
            for (auto pair : (*iter).second.getInventory())
            {
                data << pair.first << static_cast<sf::Uint32>(pair.second);
            }
            UdpManager.send(data, sf::IpAddress((*dest_iter++).second.getLocalIp()));
            //std::cout << "sent" << std::endl;
        }
        for (auto object_iter = UdpManager.getObjectDataPool().begin(); object_iter != UdpManager.getObjectDataPool().end(); ++object_iter)
        {
            sf::Packet data;
            data << static_cast<sf::Uint32>(Multiplayer::DataType::Object) << (*object_iter).second.getPosition().x << (*object_iter).second.getPosition().y << (*object_iter).second.getTime() << (*object_iter).second.getName() << (*object_iter).second.getPassability();
            UdpManager.send(data, sf::IpAddress((*iter).second.getLocalIp()));
        }
    }
}


std::vector<Multiplayer::ObjectData> load_terrain(const std::string& path)
{
    std::vector<Multiplayer::ObjectData> object_data_pool_init;
    std::ifstream infile(path);
    if (!infile.is_open())
        throw;
    int x, y;
    int name_enum, pass_enum;
    while (infile >> x >> y >> name_enum >> pass_enum)
    {
        Object::ObjectName name = static_cast<Object::ObjectName>(name_enum);
        Object::Passability pass = static_cast<Object::Passability>(pass_enum);
        object_data_pool_init.push_back(Multiplayer::ObjectData(
            sf::Vector2f(x, y),
            static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()),
            name,
            pass
        ));
    }
    return object_data_pool_init;
}


int main()
{
    //std::cout << std::flush << std::endl;
    Multiplayer::UdpManager UdpManager(sf::IpAddress::getLocalAddress(), sf::IpAddress::Any);
    {
        auto init_terrain = load_terrain("textures/terrain/terrain.txt");
        for (auto object : init_terrain)
            UdpManager.addObject(object);
    }
    while (true)
    {
        // needs another thread
        auto last_timepoint = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        
        UdpWorker(UdpManager);
 
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - last_timepoint;
        sf::sleep(((1000 / Constants::getFRAMERATE_LIMIT()) >= dt ? sf::milliseconds((1000 / Constants::getFRAMERATE_LIMIT()) - dt) : sf::Time()));
    }
    return EXIT_SUCCESS;
}
