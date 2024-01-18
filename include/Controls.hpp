#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "LinearAlgebra.hpp"
#include "Actor.hpp"
#include "Constants.hpp"
#include "Multiplayer.hpp"
#include <chrono>
#include <unordered_map>


/**
 * @brief Class for controlling the game
 * @details Controls the game, handles user input, draws the game
*/
class Controls
{
public:
    static void applyWindowSettings();
    static void drawInterfaces();
    static void setLastActionTimepoint(const Time::Time& t = Time::getTimeNow());
    static sf::Uint32 getDeltaTime();
    static void handleEvents();
    static sf::Vector2f getDirection();
    static void handleFrameStep();

    static sf::RenderWindow window;
    static Actor::User user;
    static WorldMap::ObjectMap object_map;

private:
    Controls();
    static void addEvent(const sf::Event&);

    static Multiplayer::UdpManager udp_manager;
    static sf::Vector2f direction;
    static bool left, right, up, down;
    static inline sf::Uint32 last_action_timepoint;
    static bool draw_menu, draw_inventory;
    static sf::RectangleShape menu;
    static sf::RectangleShape inventory_rect;
    static inline std::unordered_map<sf::FloatRect, Object::ObjectName> inventory;
    static inline Object::Object selected_object;
    static std::map<SocketInfo, Actor::Player> player_pool;
    static inline sf::Font font;
};
