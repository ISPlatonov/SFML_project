//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
#include <string>
#include <cstdlib>
#include "Mob.hpp"
#include "Constants.hpp"
#include "WorldMap.hpp"
//#include "LinearAlgebra.hpp"
#include "Controls.hpp"
#include "Multiplayer.hpp"

#include <chrono>


int main()
{
    Controls::applyWindowSettings();

    auto textures = Actor::load_textures("textures/actors/Guy_16x32");

    Controls::window.setView(Controls::user.getView());

    // create objects
    WorldMap::ObjectMap ObjectMap;
    //auto apple = WorldMap::Object("textures/objects/apple", sf::Vector2f(100, 100), WorldMap::Passability::background);
    
    /*
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return EXIT_FAILURE;
    sf::Text text("Hello SFML", font, 50);

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("nice_music.ogg"))
        return EXIT_FAILURE;

    // Play the music
    music.play();
    */
    
    auto UdpManager = Multiplayer::UdpManager(sf::IpAddress::getLocalAddress(), sf::IpAddress(SERVER_IP));

    //for (size_t i = 0; i < 15; ++i)
    //    Mob::mob_list.push_back(Actor::Bot(Actor::load_textures("textures/actors/Guy_16x32"), sf::Vector2f(std::rand() % WINDOW_SIZE_X, std::rand() % WINDOW_SIZE_Y) * static_cast<float>(PIXEL_SIZE)));

    Controls::setLastActionTimepoint();

    /*sf::Font font;
    if (!font.loadFromFile("textures/89speedaffair.ttf"))
        return EXIT_FAILURE;
    sf::Text text("hi", font, 30);
    text.setPosition(100, 100);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::White);*/

    sf::Packet data;
    std::map<std::string, Actor::Player> player_pool;

    while (Controls::window.isOpen())
    {
        // needs another thread
        // receiving
        for (size_t i = 0; i < player_pool.size() + UDP_PACKETS_GAP; ++i)
        {
            UdpManager.receive();
        }

        // handling player_data_pool
        for (auto iter = UdpManager.getPlayerDataPool().begin(); iter != UdpManager.getPlayerDataPool().end();)
        {
            auto time = (*iter).second.getTime();
            sf::Uint32 time_now = static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
            int ping = static_cast<int>(time_now) - static_cast<int>(time);
            if (!player_pool.count((*iter).first))
            {
                player_pool[(*iter).first] = Actor::Player((*iter).second);
                ++iter;
                continue;
            }
            else if (ping > MAX_PING)
            {
                player_pool.erase((*iter).first);
                UdpManager.removePlayerById((*iter++).first);
                continue;
            }
            else
            {
                player_pool[(*iter).first] << (*iter).second;
                ++iter;
                continue;
            }
        }

        for (auto iter : UdpManager.getObjectDataPool())
        {
            ObjectMap.addObject(iter.second);
        }
        
        data.clear();
        if (!(data << Controls::user))
            return EXIT_FAILURE;
        UdpManager.send(data);

        // Process events
        Controls::handleEvents();
        auto direction = Controls::getDirection();
        auto dt = Controls::getDeltaTime();
        Controls::user.move_dt(direction, dt, ObjectMap);
        for (size_t i = 0; i < Mob::mob_list.size(); ++i)
            Mob::mob_list[i].make_step(dt, ObjectMap);
        
        Controls::setLastActionTimepoint();
        // Clear screen
        Controls::window.clear();
        // Draw the sprite
        Controls::window.draw(WorldMap::WorldMap::map);
        for (auto iter : ObjectMap.getObjectMap(Object::Passability::background))
            Controls::window.draw(iter.second);
        for (auto iter : ObjectMap.getObjectMap(Object::Passability::impassible))
            Controls::window.draw(iter.second);
        for (const auto& bot : Mob::mob_list)
            Controls::window.draw(bot.getSprite());
        for (const auto& player : player_pool)
            Controls::window.draw(player.second.getSprite());
        Controls::window.draw(Controls::user.getSprite());
        for (auto iter : ObjectMap.getObjectMap(Object::Passability::foreground))
            Controls::window.draw(iter.second);
        // Draw the string
        //window.draw(text);

        Controls::window.setView(Controls::user.getView());
        Controls::drawMenu();
        // Update the window
        Controls::window.display();
    }
    return EXIT_SUCCESS;
}