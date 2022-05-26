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
#include <vector>
#include <set>


int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X * PIXEL_SIZE, WINDOW_SIZE_Y * PIXEL_SIZE), "SFML window");
    window.setFramerateLimit(FRAMERATE_LIMIT);

    auto textures = Actor::load_textures("textures/actors/Guy_16x32");
    Actor::User user(Actor::load_textures("textures/actors/Guy_16x32"), sf::Vector2f(0, 0) * static_cast<float>(PIXEL_SIZE));

    window.setView(user.getView());

    // define the level with an array of tile indices
    const int level[] =
    {
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 1, 1, 2, 2, 2,
        2, 2, 1, 2, 32, 32, 1, 2,
        2, 2, 1, 1, 1, 1, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 207, 207, 2, 2, 2,
        2, 2, 2, 2, 207, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 1, 1, 2, 2, 2,
        2, 2, 1, 2, 32, 32, 1, 2,
        2, 2, 1, 1, 1, 1, 1, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 207, 207, 2, 2, 2,
        2, 2, 2, 2, 207, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2,
    };

    // create the tilemap from the level definition
    WorldMap::TileMap map("textures/terrain.png", sf::Vector2u(16, 16), level, 8, 16);

    // create objects
    WorldMap::ObjectMap ObjectMap;
    //auto apple = WorldMap::Object("textures/objects/apple", sf::Vector2f(100, 100), WorldMap::Passability::background);
    ObjectMap.insert(std::make_pair<WorldMap::Passability, WorldMap::Object>(WorldMap::Passability::background, WorldMap::Object("textures/objects/apple", sf::Vector2f(1000, 1000), WorldMap::Passability::background)));
    ObjectMap.insert(std::make_pair<WorldMap::Passability, WorldMap::Object>(WorldMap::Passability::foreground, WorldMap::Object("textures/objects/grass", sf::Vector2f(200, 200), WorldMap::Passability::foreground)));
    ObjectMap.insert(std::make_pair<WorldMap::Passability, WorldMap::Object>(WorldMap::Passability::impassible, WorldMap::Object("textures/objects/wooden_wall", sf::Vector2f(400, 400), WorldMap::Passability::impassible)));
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

    for (size_t i = 0; i < 15; ++i)
        Mob::mob_list.push_back(Actor::Bot(Actor::load_textures("textures/actors/Guy_16x32"), sf::Vector2f(std::rand() % WINDOW_SIZE_X, std::rand() % WINDOW_SIZE_Y) * static_cast<float>(PIXEL_SIZE)));

    Controls::setLastActionTimepoint();
    // Start the game loop

    /*sf::Font font;
    if (!font.loadFromFile("textures/89speedaffair.ttf"))
        return EXIT_FAILURE;
    sf::Text text("hi", font, 30);
    text.setPosition(100, 100);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::White);*/

    sf::Packet data;
    std::map<std::string, Actor::Player> player_pool;

    while (window.isOpen())
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
        
        data.clear();
        if (!(data << user))
            return EXIT_FAILURE;
        UdpManager.send(data);

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            Controls::addEvent(event);
        }
        auto direction = Controls::getDirection();
        auto dt = Controls::getDeltaTime();
        user.move_dt(direction, dt, ObjectMap);
        for (size_t i = 0; i < Mob::mob_list.size(); ++i)
            Mob::mob_list[i].make_step(dt, ObjectMap);
        
        Controls::setLastActionTimepoint();
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(map);
        for (auto pass : {WorldMap::Passability::background, WorldMap::Passability::impassible})
        {
            auto range = ObjectMap.equal_range(pass);
            while (range.first != range.second)
                window.draw((*(range.first++)).second);
        }
        for (const auto& bot : Mob::mob_list)
            window.draw(bot.getSprite());
        for (const auto& player : player_pool)
            window.draw(player.second.getSprite());
        window.draw(user.getSprite());
        {
            auto range = ObjectMap.equal_range(WorldMap::Passability::foreground);
            while (range.first != range.second)
                window.draw((*(range.first++)).second);
        }
        // Draw the string
        //window.draw(text);

        window.setView(user.getView());
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}