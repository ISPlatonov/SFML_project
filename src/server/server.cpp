#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <string>
#include "Multiplayer.hpp"
#include <Constants.hpp>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
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

    for (const auto& iter : UdpManager.getPlayerDataPool())
    {
        auto time_now = Time::getTimeNow();
        int ping = static_cast<int>(time_now) - static_cast<int>(iter.second.getTime());
	    std::cout << iter.second.getSocketInfo().first << ":" << iter.second.getSocketInfo().second << std::endl;
        if (ping > Constants::getMAX_PING())
            continue;
        auto sector_data = UdpManager.checkSector(iter.second.getPosition());
        UdpManager.send(sector_data, iter.second.getSocketInfo().first, iter.second.getSocketInfo().second);
        for (const auto& dest_iter : UdpManager.getPlayerDataPool())
        {
            if (dest_iter.first == iter.first)
                continue;
            sf::Packet data;
            data << Multiplayer::DataType::Player << iter.second;
            UdpManager.send(data, dest_iter.second.getSocketInfo().first, dest_iter.second.getSocketInfo().second);
        }
    }
}


/**
 * loads terrain from file
 * 
 * @param path: path to file
 * @return vector of ObjectData
*/
static std::vector<Multiplayer::ObjectData> load_terrain(const std::string& path)
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
            Time::getTimeNow(),
            SocketInfo(0, 0), // ?
            name,
            pass
        ));
    }
    return object_data_pool_init;
}


int main()
{
    Multiplayer::UdpManager UdpManager(sf::IpAddress::getLocalAddress(), sf::IpAddress::Any);
    auto last_timepoint = std::chrono::steady_clock::now();
    {
        auto init_terrain = load_terrain("textures/terrain/terrain.txt");
        for (const auto& object : init_terrain)
            UdpManager.addObject(object);
    }
    std::cout << "server started" << std::endl;
    while (true)
    {
        last_timepoint += std::chrono::milliseconds(1000 / Constants::getFRAMERATE_LIMIT());
        UdpWorker(UdpManager);
        std::this_thread::sleep_until(last_timepoint);
    }
    return EXIT_SUCCESS;
}
