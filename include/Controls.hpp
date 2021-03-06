#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "LinearAlgebra.hpp"
#include "Actor.hpp"
#include "Constants.hpp"
#include <chrono>


class Controls
{
public:
    static void applyWindowSettings();
    static void drawMenu();
    static void setLastActionTimepoint(sf::Uint32 t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    static sf::Uint32 getDeltaTime();
    static void handleEvents();
    static sf::Vector2f getDirection();

    static sf::RenderWindow window;
    static Actor::User user;

private:
    Controls();
    static void addEvent(sf::Event);

    static sf::Vector2f direction;
    //sf::Vector2f key_to_direction(sf::Keyboard::Key key);
    static bool left, right, up, down;
    static sf::Uint32 last_action_timepoint;
    static bool draw_menu;
    static sf::RectangleShape menu;
};
