//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <chrono>
#include "Constants.hpp"
#include "Mob.hpp"
#include "WorldMap.hpp"
#include "Controls.hpp"
#include "Multiplayer.hpp"


int main()
{
    Controls::applyWindowSettings();
    Controls::window.setView(Controls::user.getView());
   
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

    //for (size_t i = 0; i < 15; ++i)
    //    Mob::mob_list.push_back(Actor::Bot(Actor::load_textures("textures/actors/Guy_16x32"), sf::Vector2f(std::rand() % WINDOW_SIZE_X, std::rand() % WINDOW_SIZE_Y) * static_cast<float>(PIXEL_SIZE)));

    Controls::setLastActionTimepoint();

    while (Controls::window.isOpen())
        Controls::handleFrameStep();
    return EXIT_SUCCESS;
}