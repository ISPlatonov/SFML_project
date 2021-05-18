//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Actor.hpp"
#include "Constants.hpp"

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
    sf::Vector2f start_point(0, 0);
    Actor user(Guy_textures, start_point);

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
            if (event.type == sf::Event::KeyPressed)
                switch (event.key.code)
                {
                    case (sf::Keyboard::A):
                        user.PressedA();
                        break;
                    case (sf::Keyboard::D):
                        user.PressedD();
                        break;
                    default:
                        break;
                }
            if (event.type == sf::Event::KeyReleased)
                switch (event.key.code)
                {
                    case (sf::Keyboard::A):
                        user.ReleasedA();
                        break;
                    case (sf::Keyboard::D):
                        user.ReleasedD();
                        break;
                    default:
                        break;
                }
        }
        user.step();
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(user.getSprite());
        // Draw the string
        //window.draw(text);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}