#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <chrono>
#include <map>
#include <string>
#include "Constants.hpp"
#include "LinearAlgebra.hpp"
#include "Multiplayer.hpp"


namespace Actor
{
    class Actor : public sf::Drawable, public sf::Transformable
    {
    public:
        Actor();
        Actor(const Actor&);
        Actor(const std::map<std::string, sf::Texture>& textures, const sf::Vector2f& position);
        void move_dt(const sf::Vector2f& direction, const sf::Uint32& dt);
        //void setPosition(sf::Vector2f p);
        void check_direction(const sf::Vector2f&);
        const sf::Vector2f& getPosition() const;
        const sf::Sprite& getSprite() const;
        const std::map<std::string, sf::Texture>& getTextures() const;

    protected:    
        sf::Sprite sprite;
        std::map<std::string, sf::Texture> textures;
        std::string direction_x;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states=sf::RenderStates()) const;
    };


    class Bot : public Actor
    {
        using Actor::Actor;

    public:
        void make_step(const sf::Uint32& dt);

    private:
        sf::Vector2f prev_move_direction = linalg::normalize(sf::Vector2f(std::rand() % 10 - 10 / 2, std::rand() % 10 - 10 / 2));
    };


    class User : public Actor
    {
    public:
        User(const std::map<std::string, sf::Texture>& textures, const sf::Vector2f& position);
        void move_dt(const sf::Vector2f& direction, const sf::Uint32& dt);
        const sf::View& getView() const;
        const int& getIp() const;
        const int& getLocalIp() const;        

    private:
        sf::View view;
        int int_ip, int_local_ip;
    };


    class Player : public Actor
    {
    public:
        // data << x << y << my_ip.toInteger() << my_local_ip.toInteger() << std::chrono::high_resolution_clock::now().time_since_epoch().count()
        Player(const std::map<std::string, sf::Texture>& textures, const sf::Vector2f& position, const int& ip, const int& local_ip, const sf::Uint32& creation_time);
        Player(const Player&);
        Player(const Multiplayer::PlayerData&);
        Player();
        const int& getIp() const;
        const int& getLocalIp() const;
        const sf::Uint32& getLastUpdateTime() const;
        void setPosition(const sf::Vector2f& position);
        void setTime(const sf::Uint32& new_time);
        void move(const sf::Vector2f& vector);

    private:
        sf::Uint32 last_update_time;
        int int_ip, int_local_ip;
    };


    std::map<std::string, sf::Texture> load_textures(std::string texture_dir_path);
    
    sf::Packet& operator <<(sf::Packet& packet, const User& user);
    Player& operator <<(Player& player, const Multiplayer::PlayerData& player_data);
}