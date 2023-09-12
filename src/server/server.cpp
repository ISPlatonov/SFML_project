#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <string>
#include "Multiplayer.hpp"
#include <Constants.hpp>
#include <chrono>
#include <thread>
#include <vector>
//#include <iostream>
#include <fstream>


/**
 * @brief Class for handling client-server connection on server side
*/
void UdpWorker(Multiplayer::UdpManager& UdpManager)
{
    for (size_t i = 0; i < UdpManager.getPlayerDataPool().size() + Constants::getUDP_PACKETS_GAP(); ++i)
    {
        UdpManager.receive();
    }

    for (auto iter : UdpManager.getPlayerDataPool())
    {
        //std::cout << "unpacking player from pool: " << (*iter).first << std::endl;
        auto time_now = static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
        iter.second.setTime(time_now);
        int ping = static_cast<int>(time_now) - static_cast<int>(iter.second.getTime());
        //std::cout << "id: " << (*iter).first << ", last timepoint: " << std::to_string(time) << std::endl;

        if (ping > Constants::getMAX_PING())
        {
            //std::cout << "reached MAX_PING" << std::endl;
            //UdpManager.removePlayerById((*iter++).first);
            continue;
        }
        auto sector_data = UdpManager.checkSector(iter.second.getPosition());
        UdpManager.send(sector_data, sf::IpAddress(iter.second.getLocalIp()));
        for (auto dest_iter = UdpManager.getPlayerDataPool().begin(); dest_iter != UdpManager.getPlayerDataPool().end();)
        {
            //std::cout << "sending " << (*iter).first << " data to " << (*dest_iter).first << std::endl;
            if (dest_iter->first == iter.first)
            {
                //std::cout << "*dest_iter == *iter" << std::endl;
                ++dest_iter;
                //std::cout << "made ++dest_iter" << std::endl;
                continue;
            }
            sf::Packet data;
            data << Multiplayer::DataType::Player << iter.second;
            
            UdpManager.send(data, sf::IpAddress(dest_iter++->second.getLocalIp()));
            //std::cout << "sent" << std::endl;
        }
    }
}


/**
 * loads terrain from file
 * 
 * @param path: path to file
 * @return vector of ObjectData
*/
std::vector<Multiplayer::ObjectData> load_terrain(const std::string& path)
{
    std::vector<Multiplayer::ObjectData> object_data_pool_init;
    std::ifstream infile(path);
    if (!infile.is_open())
        throw std::invalid_argument("invalid path");
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
    auto last_timepoint = std::chrono::steady_clock::now();
    {
        auto init_terrain = load_terrain("textures/terrain/terrain.txt");
        for (const auto& object : init_terrain)
            UdpManager.addObject(object);
    }
    while (true)
    {
        // needs another thread
        last_timepoint += std::chrono::milliseconds(1000 / Constants::getFRAMERATE_LIMIT());
        UdpWorker(UdpManager);
        std::this_thread::sleep_until(last_timepoint);
    }
    return EXIT_SUCCESS;
}
