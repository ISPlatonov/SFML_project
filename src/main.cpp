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


int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X * PIXEL_SIZE, WINDOW_SIZE_Y * PIXEL_SIZE), "SFML window");
    window.setFramerateLimit(FRAMERATE_LIMIT);

    window.setView(Mob::user.getView());

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

    for (size_t i = 0; i < 15; ++i)
        Mob::mob_list.push_back(Bot(load_textures("textures/actors/Guy_16x32"), sf::Vector2f(std::rand() % WINDOW_SIZE_X, std::rand() % WINDOW_SIZE_Y) * static_cast<float>(PIXEL_SIZE)));

    Controls::setLastActionTimepoint();
    // Start the game loop
    while (window.isOpen())
    {
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
        Mob::user.move_dt(direction, dt);
        for (size_t i = 0; i < Mob::mob_list.size(); ++i)
            Mob::mob_list[i].make_step(dt);
        
        Controls::setLastActionTimepoint();
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(map);
        for (auto bot : Mob::mob_list)
            window.draw(bot.getSprite());
        window.draw(Mob::user.getSprite());
        // Draw the string
        //window.draw(text);

        window.setView(Mob::user.getView());
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}