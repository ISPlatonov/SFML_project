//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
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

    auto textures = load_textures("textures/actors/Guy_16x32");
    Actor::User user(load_textures("textures/actors/Guy_16x32"), sf::Vector2f(0, 0) * static_cast<float>(PIXEL_SIZE));

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
    TileMap map;
    if (!map.load("textures/terrain.png", sf::Vector2u(16, 16), level, 8, 16))
        return EXIT_FAILURE;

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


    sf::UdpSocket socket;
    sf::UdpSocket socket_send;
    unsigned short port = 55000;
    unsigned short port_send = 54000;
    auto broadcast_ip = "77.73.71.158";
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

    for (size_t i = 0; i < 15; ++i)
        Mob::mob_list.push_back(Actor::Bot(load_textures("textures/actors/Guy_16x32"), sf::Vector2f(std::rand() % WINDOW_SIZE_X, std::rand() % WINDOW_SIZE_Y) * static_cast<float>(PIXEL_SIZE)));

    Controls::setLastActionTimepoint();
    // Start the game loop

    sf::Font font;
    if (!font.loadFromFile("textures/89speedaffair.ttf"))
        return EXIT_FAILURE;
    sf::Text text(my_local_ip.toString(), font, 30);
    text.setPosition(100, 100);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::White);

    sf::Packet data;
    sf::Vector2f multiplayer_position;
    // int ip, Actor::Player player
    std::map<std::string, Actor::Player> player_pool;
    std::set<std::string> ip_pool;

    while (window.isOpen())
    {
        // needs another thread
        //Mob::multiplayers_list.clear();
        for (size_t i = 0; i < player_pool.size() + 10; ++i)
        {
            //data.clear();
            int msg_local_ip, msg_ip;
            sf::Uint32 sent_time;
            sf::Vector2f new_position;
            auto status = socket.receive(data, address_receive, port_send);
            if (status != sf::Socket::Status::Done)
                continue;
            data >> new_position.x >> new_position.y >> msg_ip >> msg_local_ip >> sent_time;
            text.setString("got packet");
            //text.setString(std::to_string(data.getDataSize()));
            if (msg_ip == my_ip.toInteger() && msg_local_ip == my_local_ip.toInteger())
                continue;
            
            auto id = sf::IpAddress(msg_ip).toString() + sf::IpAddress(msg_local_ip).toString();

            sf::Uint32 time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            sf::Uint32 ping = time_now - sent_time;
            text.setString(std::to_string(time_now) + '\n' + std::to_string(ping));
            if (player_pool.count(id))
                if (ping > 5000000)
                {
                    player_pool.erase(id);
                    ip_pool.erase(id);
                }
                else
                    player_pool[id].updatePosition(new_position);
            else
                if (ping > 5000000)
                    continue;
                else
                {
                    player_pool[id] = Actor::Player(textures, new_position, msg_ip, msg_local_ip, sent_time);
                    ip_pool.insert(id);
                }

            /*if (std::chrono::high_resolution_clock::now().time_since_epoch().count() - sent_time > 1000000000)
                if (player_pool.count(msg_local_ip))
                    player_pool.erase(msg_local_ip);
                else
                    continue;
            */
            //Mob::multiplayers_list.push_back(Actor::Actor(textures, multiplayer_position));
        }
        for (size_t i = 0; i < Mob::multiplayers_list.size(); ++i)
            Mob::multiplayers_list[i].setPosition(multiplayer_position);
        
        
        data.clear();
        if (!(data << user))
            return EXIT_FAILURE;
        socket_send.send(data, address_send, port);

        //text.setString(std::to_string(multiplayer_position.x) + ' ' + std::to_string(multiplayer_position.y));
        //text.setString(/*my_ip.toString() + ", " +*/ my_local_ip.toString());
        


        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            
            // boba
            Controls::addEvent(event);
        }
        auto direction = Controls::getDirection();
        
        auto dt = Controls::getDeltaTime();
        user.move_dt(direction, dt);
        for (size_t i = 0; i < Mob::mob_list.size(); ++i)
            Mob::mob_list[i].make_step(dt);
        
        Controls::setLastActionTimepoint();
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(map);
        for (const auto& bot : Mob::mob_list)
            window.draw(bot.getSprite());
        for (const auto& id : ip_pool)
            window.draw(player_pool[id].getSprite());
        window.draw(user.getSprite());
        // Draw the string
        window.draw(text);

        window.setView(user.getView());
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}