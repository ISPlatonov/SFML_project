#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#define PIXEL_SIZE size_t(10)
#define WINDOW_SIZE_X size_t(sf::VideoMode::getDesktopMode().width / PIXEL_SIZE)
#define WINDOW_SIZE_Y size_t(sf::VideoMode::getDesktopMode().height / PIXEL_SIZE)
#define WINDOW_SIZE sf::Vector2u(WINDOW_SIZE_X, WINDOW_SIZE_Y)
//#define WORK_SPACE sf::Rect(16, 8, 96, 48)
#define FRAMERATE_LIMIT size_t(60)
#define STEP_SIZE_MULTIPLIER float(.05)

#define TILE_SIZE PIXEL_SIZE

// Network
#define PORT_LISTEN size_t(55000)
#define PORT_SEND size_t(55001)
#define SERVER_IP std::string("77.73.71.158")
#define MAX_PING int(5000)
#define UDP_PACKETS_GAP size_t(1000)