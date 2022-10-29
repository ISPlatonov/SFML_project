#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "LinearAlgebra.hpp"
#include "Actor.hpp"
#include "Constants.hpp"
#include "Multiplayer.hpp"
#include <chrono>


/**
 * @brief Class for controlling the game
 * @details Controls the game, handles user input, draws the game
*/
class Controls
{
public:
    static void applyWindowSettings();
    static void drawInterfaces();
    static void setLastActionTimepoint(sf::Uint32 t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    static sf::Uint32 getDeltaTime();
    static void handleEvents();
    static sf::Vector2f getDirection();
    static void handleFrameStep();

    static sf::RenderWindow window;
    static Actor::User user;
    static WorldMap::ObjectMap object_map;

private:
    Controls();
    static void addEvent(sf::Event);

    static Multiplayer::UdpManager udp_manager;
    static sf::Vector2f direction;
    //sf::Vector2f key_to_direction(sf::Keyboard::Key key);
    static bool left, right, up, down;
    static sf::Uint32 last_action_timepoint;
    static bool draw_menu, draw_inventory;
    static sf::RectangleShape menu;
    static sf::RectangleShape inventory_rect;
    static Object::Object selected_object;
    static std::map<std::string, Actor::Player> player_pool;
    static sf::Font font;
};
