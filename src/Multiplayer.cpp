#include "Multiplayer.hpp"


namespace Multiplayer
{

}


namespace sf
{
    Packet& operator <<(Packet& packet, const Actor::User& user)
    {
        auto position = user.getSprite().getPosition();
        sf::Uint32 time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        return packet << position.x << position.y << user.getIp() << time;
    }
}