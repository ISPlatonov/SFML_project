#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <chrono>
#include <map>
#include <string>
#include <unordered_map>
#include "Constants.hpp"
#include "LinearAlgebra.hpp"
#include "Multiplayer.hpp"
#include "WorldMap.hpp"
#include "Object.hpp"


namespace Actor
{
    class Actor : public sf::Drawable, public sf::Transformable
    {
    public:
        Actor();
        Actor(const Actor&);
        Actor(const std::map<std::string, sf::Texture>& textures, const sf::Vector2f& position);
        Actor(std::map<std::string, sf::Texture>&& textures, sf::Vector2f&& position);
        sf::Vector2f&& move_dt(const sf::Vector2f& direction, const sf::Uint32& dt, const WorldMap::ObjectMap& ObjectMap = {});
        //void setPosition(sf::Vector2f p);
        void check_direction(const sf::Vector2f&);
        const sf::Vector2f& getPosition() const;
        const sf::Sprite& getSprite() const;
        const std::map<std::string, sf::Texture>& getTextures() const;
        const size_t objectNumber(Object::ObjectName) const;
        const size_t addObject(Object::ObjectName);
        const size_t removeObject(Object::ObjectName);

    protected:    
        sf::Sprite sprite;
        std::map<std::string, sf::Texture> textures;
        std::string direction_x;
        std::unordered_map<Object::ObjectName, size_t> inventory;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states=sf::RenderStates()) const;
    };


    class Bot : public virtual Actor
    {
    public:
        using Actor::Actor;
        void make_step(const sf::Uint32& dt, const WorldMap::ObjectMap& ObjectMap = {});

    private:
        sf::Vector2f prev_move_direction = linalg::normalize(sf::Vector2f(std::rand() % 10 - 10 / 2, std::rand() % 10 - 10 / 2));
    };


    class User : public virtual Actor
    {
    public:
        User(const std::map<std::string, sf::Texture>& textures, const sf::Vector2f& position);
        User(std::map<std::string, sf::Texture>&& textures, sf::Vector2f&& position);
        sf::Vector2f&& move_dt(const sf::Vector2f& direction, const sf::Uint32& dt, const WorldMap::ObjectMap& ObjectMap = {});
        const sf::View& getView() const;
        const int& getIp() const;
        const int& getLocalIp() const;        

    private:
        sf::View view;
        int int_ip, int_local_ip;
    };


    class Player : public virtual Actor
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


    std::map<std::string, sf::Texture>&& load_textures(const std::string& texture_dir_path);
    
    sf::Packet& operator <<(sf::Packet& packet, const User& user);
    Player& operator <<(Player& player, const Multiplayer::PlayerData& player_data);
}