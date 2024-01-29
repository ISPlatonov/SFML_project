#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Constants.hpp"
#include "LinearAlgebra.hpp"
#include "Multiplayer.hpp"
#include "WorldMap.hpp"
#include "Object.hpp"
#include <string>
#include <unordered_map>


namespace Actor
{
    enum class Direction {
        left,
        right,
        up,
        down
    };


    /**
     * @brief Base class for all actors
    */
    class Actor : public sf::Drawable, public sf::Transformable
    {
    public:
        Actor() {}
        Actor(const Actor& actor) : Actor(actor.getPosition(), actor.getInventory()) {}
        Actor(const sf::Vector2f& position, const Multiplayer::Inventory& inventory);
        Actor(sf::Vector2f&& position, Multiplayer::Inventory&& inventory);
        sf::Vector2f move_dt(const sf::Vector2f& direction, const sf::Uint32& dt, WorldMap::ObjectMap& ObjectMap);
        void check_direction(const sf::Vector2f&);
        inline const sf::Vector2f& getPosition() const { return getSprite().getPosition(); }
        inline const sf::Sprite& getSprite() const { return sprite; }
        inline const Multiplayer::Inventory& getInventory() const { return inventory; }
        const size_t addObject(Object::ObjectName);
        size_t removeObject(Object::ObjectName);
        inline void setInventory(const Multiplayer::Inventory& new_inventory) { inventory = new_inventory; }

    protected:    
        sf::Sprite sprite;
        static inline std::unordered_map<Direction, sf::Texture> textures{};
        Direction direction;
        Multiplayer::Inventory inventory;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states=sf::RenderStates()) const;
    
    private:
        void load_textures(const std::string& texture_dir_path);
    };


    /**
     * @brief Class for bot
     * @details Bot is an actor that can be controlled by AI
    */
    class Bot : public virtual Actor
    {
    public:
        using Actor::Actor;
        void make_step(const sf::Uint32& dt, WorldMap::ObjectMap& ObjectMap);

    private:
        sf::Vector2f prev_move_direction = linalg::normalize(sf::Vector2f(std::rand() % 10 - 10 / 2, std::rand() % 10 - 10 / 2));
    };


    /**
     * @brief Class for user
     * @details User is an actor that can be controlled by user
    */
    class User : public virtual Actor
    {
    public:
        User(const sf::Vector2f& position, const unsigned int& port, const Multiplayer::Inventory& inventory = {});
        User(sf::Vector2f&& position, unsigned int&& port, Multiplayer::Inventory&& inventory = {});
        sf::Vector2f move_dt(const sf::Vector2f& direction, const sf::Uint32& dt, WorldMap::ObjectMap& ObjectMap);
        inline const sf::View& getView() const { return view; }
        inline int getIp() const { return int_ip; }
        inline int getPort() const { return int_port; }

    private:
        sf::View view;
        int int_ip;
        unsigned int int_port;
    };


    /**
     * @brief Class for player
     * @details Player is an actor that can be controlled by another user in multiplayer
    */
    class Player : public virtual Actor
    {
    public:
        Player() : Actor() {}
        Player(const sf::Vector2f& pos, const PlayerId& p_id, const sf::Uint32& ct, const Multiplayer::Inventory& inv)
            : Actor(pos, inv), player_id(p_id), last_update_time(ct) {}
        Player(const Player& p) : Player(p.getPosition(), p.getId(), p.getLastUpdateTime(), p.getInventory()) {}
        Player(const Multiplayer::PlayerData& pd)
            : Player(pd.getPosition() * static_cast<float>(Constants::getPIXEL_SIZE()), pd.getId(), pd.getTime(), pd.getInventory()) {}
        inline PlayerId getId() const { return player_id; }
        inline Time::Time getLastUpdateTime() const { return last_update_time; }
        void setPosition(const sf::Vector2f& position);
        inline void setTime(const sf::Uint32& new_time) { last_update_time = new_time; }
        void move(const sf::Vector2f& vector);

    private:
        sf::Uint32 last_update_time;
        PlayerId player_id;
    };


    sf::Packet& operator <<(sf::Packet& packet, const User& user);
    Player& operator <<(Player& player, const Multiplayer::PlayerData& player_data);
}