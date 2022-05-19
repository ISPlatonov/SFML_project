#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <chrono>
#include "Actor.hpp"


namespace Multiplayer
{
    /*class Packet : public sf::Packet
    {
        virtual const void* onSend(std::size_t& size)
        {
            const void* srcData = getData();
            std::size_t srcSize = getDataSize();
            return compressTheData(srcData, srcSize, &size); // this is a fake function, of course :)
        }


        virtual void onReceive(const void* data, std::size_t size)
        {
            std::size_t dstSize;
            const void* dstData = uncompressTheData(data, size, &dstSize); // this is a fake function, of course :)
            append(dstData, dstSize);
        }
    };*/


    



    /*
    sf::Packet& operator >>(sf::Packet& packet, Actor::Player& player)
    {
        float x, y;
        int ip;
        sf::Uint32 time;
        return packet >> x >> y >> ip >> time;
    }*/
}


namespace sf
{
    // data << x << y << my_ip.toInteger() << my_local_ip.toInteger() << std::chrono::steady_clock::now().time_since_epoch().count()
    Packet& operator <<(Packet& packet, const Actor::User& user);
}
