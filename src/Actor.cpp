#include "Actor.hpp"


namespace Actor
{
    Actor::Actor()
    {

    }


    Actor::Actor(const std::map<std::string, sf::Texture>& textures, const sf::Vector2f& position)
    {
        this->textures = textures;
        direction_x = "right";
        sprite.setPosition(position);
        sprite.setTexture(this->textures[direction_x]);
        sprite.setScale(PIXEL_SIZE, PIXEL_SIZE);
    }


    Actor::Actor(std::map<std::string, sf::Texture>&& textures, sf::Vector2f&& position)
    {
        this->textures = std::move(textures);
        direction_x = "right";
        sprite.setPosition(position);
        sprite.setTexture(this->textures[direction_x]);
        sprite.setScale(PIXEL_SIZE, PIXEL_SIZE);
    }


    const sf::Sprite& Actor::getSprite() const
    {
        return sprite;
    }


    void Actor::check_direction(const sf::Vector2f& direction)
    {
        if (direction.x > 0 && direction_x != "right")
            direction_x = "right";
        else if (direction.x < 0 && direction_x != "left")
            direction_x = "left";
        else
            return;
        sprite.setTexture(textures[direction_x]);
    }


    sf::Vector2f&& Actor::move_dt(const sf::Vector2f& direction, const sf::Uint32& dt, const WorldMap::ObjectMap& ObjectMap)
    {
        check_direction(direction); 
        auto v = new sf::Vector2f(linalg::normalize(direction) * static_cast<float>(dt) * STEP_SIZE_MULTIPLIER * static_cast<float>(PIXEL_SIZE));
        for (const auto& object : ObjectMap)
        {
            object.second.check_collision(*v, this->getSprite().getGlobalBounds());
        }
        sprite.move(*v);
        return std::move(*v);
    }


    void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(sprite, states);
    }


    sf::Vector2f&& User::move_dt(const sf::Vector2f& direction, const sf::Uint32& dt, const WorldMap::ObjectMap& ObjectMap)
    {
        auto v = new sf::Vector2f(Actor::move_dt(direction, dt, ObjectMap));
        view.move(*v);
        return std::move(*v);
    }


    User::User(const std::map<std::string, sf::Texture>& textures, const sf::Vector2f& position) : Actor(textures, position)
    {
        // make it static...
        view = sf::View(position + (getSprite().getLocalBounds().getSize() / 2.f * static_cast<float>(PIXEL_SIZE)), static_cast<sf::Vector2f>(WINDOW_SIZE * static_cast<float>(PIXEL_SIZE)));
        int_ip = sf::IpAddress::getPublicAddress(sf::seconds(5.f)).toInteger();
        int_local_ip = sf::IpAddress::getLocalAddress().toInteger();
    }


    User::User(std::map<std::string, sf::Texture>&& textures, sf::Vector2f&& position) : Actor(textures, position)
    {
        view = std::move(sf::View(position + (getSprite().getLocalBounds().getSize() / 2.f * static_cast<float>(PIXEL_SIZE)), static_cast<sf::Vector2f>(WINDOW_SIZE * static_cast<float>(PIXEL_SIZE))));
        int_ip = std::move(sf::IpAddress::getPublicAddress(sf::seconds(5.f)).toInteger());
        int_local_ip = std::move(sf::IpAddress::getLocalAddress().toInteger());
    }


    const sf::View& User::getView() const
    {
        return view;
    }


    void Bot::make_step(const sf::Uint32& dt, const WorldMap::ObjectMap& ObjectMap)
    {
        prev_move_direction = linalg::normalize(linalg::normalize(sf::Vector2f(std::rand() % 11 - 11 / 2, std::rand() % 11 - 11 / 2)) * .2f + prev_move_direction * .8f);
        move_dt(prev_move_direction, dt, ObjectMap);
    }


    Player::Player(const std::map<std::string, sf::Texture>& textures, const sf::Vector2f& position, const int& int_ip, const int& int_local_ip, const sf::Uint32& creation_time) : Actor(textures, position)
    {
        this->int_ip = int_ip;
        this->int_local_ip = int_local_ip;
        last_update_time = creation_time;
    }


    const int& Player::getIp() const
    {
        return int_ip;
    }


    const int& Player::getLocalIp() const
    {
        return int_local_ip;
    }


    const sf::Uint32& Player::getLastUpdateTime() const
    {
        return last_update_time;
    }


    const int& User::getIp() const
    {
        return int_ip;
    }


    const int& User::getLocalIp() const
    {
        return int_local_ip;
    }


    void Player::setPosition(const sf::Vector2f& position)
    {
        auto prev_position = getPosition();
        auto move_vector = position - prev_position;
        move(move_vector);
    }


    Player::Player(const Player& player) : Actor(player)
    {
        // Player(std::map<std::string, sf::Texture> textures, sf::Vector2f position, int int_ip, sf::Uint32 creation_time);
        int_ip = player.getIp();
        int_local_ip = player.getLocalIp();
        last_update_time = player.getLastUpdateTime();
    }


    Actor::Actor(const Actor& actor) : Actor(actor.getTextures(), actor.getPosition())
    {
        
    }


    const sf::Vector2f& Actor::getPosition() const
    {
        return getSprite().getPosition();
    }


    Player::Player() : Actor()
    {

    }


    const std::map<std::string, sf::Texture>& Actor::getTextures() const
    {
        return textures;
    }


    void Player::move(const sf::Vector2f& vector)
    {
        check_direction(vector); 
        sprite.move(vector);
    }


    sf::Packet& operator <<(sf::Packet& packet, const User& user)
    {
        auto position = user.getPosition();
        sf::Uint32 time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        return packet << position.x << position.y << user.getIp() << user.getLocalIp() << time;
    }


    void Player::setTime(const sf::Uint32& new_time)
    {
        last_update_time = new_time;
    }


    std::map<std::string, sf::Texture>&& load_textures(const std::string& texture_dir_path)
    {
        // Load a sprite to display
        std::map<std::string, sf::Texture>* textures = new std::map<std::string, sf::Texture>;
        (*textures)["left"] = sf::Texture();
        (*textures)["right"] = sf::Texture();
        (*textures)["left"].loadFromFile(texture_dir_path + "/left.png");
        (*textures)["right"].loadFromFile(texture_dir_path + "/right.png");

        return std::move(*textures);
    }


    // ip and local_ip won't be changed
    Player& operator <<(Player& player, const Multiplayer::PlayerData& player_data)
    {
        player.setPosition(player_data.getPosition());
        player.setTime(player_data.getTime());
        return player;
    }


    // !!! rewrite PlayerData!!!
    Player::Player(const Multiplayer::PlayerData& player_data) : Player(load_textures("textures/actors/Guy_16x32"), player_data.getPosition(), player_data.getIp(), player_data.getLocalIp(), player_data.getTime())
    {

    }
}