#pragma once

#ifdef CLIENT
    #include <SFML/Window.hpp>
#endif
#include <string>
#include <fstream>


/**
 * @brief This class is used to read settings from file
*/
class Constants
{
public:
    static inline void load_constants()
    {
        std::ifstream infile("textures/constants.txt");
        if (!infile.is_open())
            throw;
        infile >> PIXEL_SIZE;
        TILE_SIZE = PIXEL_SIZE;
        #ifdef CLIENT
            WINDOW_SIZE_X = sf::VideoMode::getDesktopMode().width / PIXEL_SIZE;
            WINDOW_SIZE_Y = sf::VideoMode::getDesktopMode().height / PIXEL_SIZE;
            WINDOW_SIZE = sf::Vector2f(WINDOW_SIZE_X, WINDOW_SIZE_Y);
        #endif
        infile >> FRAMERATE_LIMIT;
        infile >> std::boolalpha >> ENABLE_VSYNC;
        infile >> MAX_RECURSION_DEPTH;
        infile >> VIEW_RADIUS;
        infile >> PORT_LISTEN;
        infile >> PORT_SEND;
        infile >> SERVER_IP;
        infile >> MAX_PING;
        infile >> UDP_PACKETS_GAP;
        loaded = true;
    }
    static inline const size_t& getPIXEL_SIZE()
    {
        while (!loaded)
            load_constants();
        return PIXEL_SIZE; 
    }
    static inline const bool& getFULLSCREEN()
    {
        while (!loaded)
            load_constants();
        return FULLSCREEN;
    }
    #ifdef CLIENT
        static inline const size_t& getWINDOW_SIZE_X()
        {
            while (!loaded)
                load_constants();
            return WINDOW_SIZE_X;
        }
        static inline const size_t& getWINDOW_SIZE_Y()
        {
            while (!loaded)
                load_constants();
            return WINDOW_SIZE_Y;
        }
        static inline const sf::Vector2f& getWINDOW_SIZE()
        {
            while (!loaded)
                load_constants();
            return WINDOW_SIZE;
        }
    #endif
    static inline const size_t& getFRAMERATE_LIMIT()
    {
        while (!loaded)
            load_constants();
        return FRAMERATE_LIMIT;
    }
    static inline const bool& getENABLE_VSYNC()
    {
        while (!loaded)
            load_constants();
        return ENABLE_VSYNC;
    }
    static inline const float& getSTEP_SIZE_MULTIPLIER()
    {
        while (!loaded)
            load_constants();
        return STEP_SIZE_MULTIPLIER;
    }
    static inline const size_t& getMAX_RECURSION_DEPTH()
    {
        while (!loaded)
            load_constants();
        return MAX_RECURSION_DEPTH;
    }
    static inline const int& getVIEW_RADIUS()
    {
        while (!loaded)
            load_constants();
        return VIEW_RADIUS;
    }
    static inline const size_t& getTILE_SIZE()
    {
        while (!loaded)
            load_constants();
        return TILE_SIZE;
    }
    static inline const size_t& getPORT_LISTEN()
    {
        while (!loaded)
            load_constants();
        return PORT_LISTEN;
    }
    static inline const size_t& getPORT_SEND()
    {
        while (!loaded)
            load_constants();
        return PORT_SEND;
    }
    static inline const std::string& getSERVER_IP()
    {
        while (!loaded)
            load_constants();
        return SERVER_IP;
    }
    static inline const int& getMAX_PING()
    {
        while (!loaded)
            load_constants();
        return MAX_PING;
    }
    static inline const size_t& getUDP_PACKETS_GAP()
    {
        while (!loaded)
            load_constants();
        return UDP_PACKETS_GAP;
    }

private:
    Constants();

    static inline bool loaded = false;
    static inline size_t PIXEL_SIZE = 10;
    static inline bool FULLSCREEN = true;
    #ifdef CLIENT
        static inline size_t WINDOW_SIZE_X = sf::VideoMode::getDesktopMode().width / PIXEL_SIZE;
        static inline size_t WINDOW_SIZE_Y = sf::VideoMode::getDesktopMode().height / PIXEL_SIZE;
        static inline sf::Vector2f WINDOW_SIZE = sf::Vector2f(WINDOW_SIZE_X, WINDOW_SIZE_Y);
    #endif
    static inline size_t FRAMERATE_LIMIT = 60;
    static inline bool ENABLE_VSYNC = true;
    static inline float STEP_SIZE_MULTIPLIER = .05;
    static inline size_t MAX_RECURSION_DEPTH = 15;
    static inline size_t TILE_SIZE = PIXEL_SIZE;
    static inline int VIEW_RADIUS = 10;
    // Network
    static inline size_t PORT_LISTEN = 55000;
    static inline size_t PORT_SEND = 55001;
    static inline std::string SERVER_IP = "77.73.71.158";
    static inline int MAX_PING = 5000;
    static inline size_t UDP_PACKETS_GAP = 1000;
};