#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Actor.hpp"

#define PIXEL_SIZE size_t(10)
#define WINDOW_SIZE_X size_t(128)
#define WINDOW_SIZE_Y size_t(64)
#define WORK_SPACE sf::Rect(16, 8, 96, 48)

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE_X * PIXEL_SIZE, WINDOW_SIZE_Y * PIXEL_SIZE), "SFML window");
    
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("textures/Guy_16x32_right.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);
    sprite.scale(PIXEL_SIZE, PIXEL_SIZE);
    sprite.setPosition(0, (WINDOW_SIZE_Y - sprite.getLocalBounds().height) * PIXEL_SIZE);

    Actor user(sprite);

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