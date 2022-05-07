//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Actor.hpp"
#include "Constants.hpp"
#include "WorldMap.hpp"
//#include "LinearAlgebra.hpp"
#include "Controls.hpp"


int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X * PIXEL_SIZE, WINDOW_SIZE_Y * PIXEL_SIZE), "SFML window");
    window.setFramerateLimit(FRAMERATE_LIMIT);
    
    // Load a sprite to display
    std::map<std::string, sf::Texture> Guy_textures;
    Guy_textures["left"] = sf::Texture();
    Guy_textures["right"] = sf::Texture();
    //texture_Guy_16x32_right;
    if (!Guy_textures["left"].loadFromFile("textures/Guy_16x32_left.png"))
        return EXIT_FAILURE;
    if (!Guy_textures["right"].loadFromFile("textures/Guy_16x32_right.png"))
        return EXIT_FAILURE;
    /*sf::Sprite sprite(texture);
    sprite.scale(PIXEL_SIZE, PIXEL_SIZE);
    sprite.setPosition(0, (WINDOW_SIZE_Y - sprite.getLocalBounds().height) * PIXEL_SIZE);
    */
    sf::Vector2f start_point = sf::Vector2f(8, 4) * static_cast<float>(PIXEL_SIZE);
    Actor user(Guy_textures, start_point);

    window.setView(user.view);

    // define the level with an array of tile indices
    const int level[] =
    {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0,
        1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
        0, 1, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0,
        0, 1, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0,
        2, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1,
    };

    // create the tilemap from the level definition
    TileMap map;
    if (!map.load("textures/tileset.png", sf::Vector2u(32, 32), level, 16, 8))
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
        if (direction != sf::Vector2f())
        {
            auto dt = Controls::getDeltaTime();
            user.move(direction, dt);
        }
        Controls::setLastActionTimepoint();
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(map);
        window.draw(user.getSprite());
        // Draw the string
        //window.draw(text);

        window.setView(user.view);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}