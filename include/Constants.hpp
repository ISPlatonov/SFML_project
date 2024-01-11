#pragma once

#ifdef CLIENT
    #include <SFML/Window.hpp>
#endif
#include <SFML/Network.hpp>
#include <string>
#include <fstream>
#include <random>
#include <chrono>


/**
 * @brief This class is used to store player unique id
*/
class PlayerId {
public:
    PlayerId() {}
    PlayerId(size_t ip, size_t port, size_t timestamp, size_t key) : ip(ip), port(port), timestamp(timestamp), key(key) {}
    
    inline size_t getIp() const { return ip; }
    inline size_t getPort() const { return port; }
    inline size_t getTimestamp() const { return timestamp; }
    inline size_t getKey() const { return key; }

    inline bool operator==(const PlayerId& other) const {
        return ip == other.ip && port == other.port && timestamp == other.timestamp && key == other.key;
    }

private:
    size_t ip = 0;
    size_t port = 0;
    size_t timestamp = 0;
    size_t key = 0;
};


inline sf::Packet& operator <<(sf::Packet& packet, const PlayerId& id)
{
    packet << static_cast<sf::Uint64>(id.getIp()) << static_cast<sf::Uint64>(id.getPort()) << static_cast<sf::Uint64>(id.getTimestamp()) << static_cast<sf::Uint64>(id.getKey());
    return packet;
}


typedef std::pair<sf::IpAddress, unsigned short> SocketInfo;


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
        infile.close();
        std::ifstream infile_id("textures/id.txt");
        if (!infile_id.is_open()) {
            std::ofstream outfile("textures/id.txt");
            sf::IpAddress ip_global = sf::IpAddress::getPublicAddress();
            auto socket = sf::UdpSocket();
            while (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done);
            auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            size_t random_key = std::random_device()();
            outfile << ip_global.toInteger() << std::endl
                    << socket.getLocalPort() << std::endl
                    << timestamp << std::endl
                    << random_key << std::endl;
            outfile.close();
            infile_id.open("textures/id.txt");
        }
        size_t id_ip, id_port, id_timestamp, id_key;
        infile_id >> id_ip >> id_port >> id_timestamp >> id_key;
        id = {id_ip, id_port, id_timestamp, id_key};
        infile_id.close();
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
    static inline const PlayerId& getID() {
        while (!loaded)
            load_constants();
        return id;
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
    static inline PlayerId id = {0, 0, 0, 0};
};