#include "Actor.hpp"


namespace Actor
{
    Actor::Actor()
    {

    }


    Actor::Actor(std::map<std::string, sf::Texture> textures, sf::Vector2f position)
    {
        this->textures = textures;
        sprite.setPosition(position);
        this->direction_x = "right";
        sprite.setTexture(Actor::textures[this->direction_x]);
        sprite.setScale(PIXEL_SIZE, PIXEL_SIZE);
        /*
        left = 0, 
        right = 0, 
        up = 0, 
        down = 0;
        last_action_timepoint = 0;
        */
    }


    const sf::Sprite& Actor::getSprite() const
    {
        return sprite;
    }


    void Actor::check_direction(sf::Vector2f direction)
    {
        if (direction.x > 0 && direction_x != "right")
            direction_x = "right";
        else if (direction.x < 0 && direction_x != "left")
            direction_x = "left";
        else
            return;
        sprite.setTexture(Actor::textures[direction_x]);
    }


    void Actor::move_dt(sf::Vector2f direction, sf::Uint32 dt)
    {
        check_direction(direction); 
        auto v = linalg::normalize(direction) * static_cast<float>(dt) * STEP_SIZE_MULTIPLIER * static_cast<float>(PIXEL_SIZE);
        sprite.move(v);
    }


    void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(sprite, states);
    }


    void User::move_dt(sf::Vector2f direction, sf::Uint32 dt)
    {
        check_direction(direction); 
        auto v = linalg::normalize(direction) * static_cast<float>(dt) * STEP_SIZE_MULTIPLIER * static_cast<float>(PIXEL_SIZE);
        sprite.move(v);
        view.move(v);
    }


    User::User(std::map<std::string, sf::Texture> textures, sf::Vector2f position) : Actor{textures, position}
    {
        // make it static...
        view = sf::View(position + (getSprite().getLocalBounds().getSize() / 2.f), static_cast<sf::Vector2f>(WINDOW_SIZE * static_cast<unsigned int>(PIXEL_SIZE)));
        int_ip = sf::IpAddress::getPublicAddress(sf::seconds(5.f)).toInteger();
        int_local_ip = sf::IpAddress::getLocalAddress().toInteger();
    }


    const sf::View& User::getView() const
    {
        return view;
    }


    void Bot::make_step(sf::Uint32 dt)
    {
        prev_move_direction = linalg::normalize(linalg::normalize(sf::Vector2f(std::rand() % 11 - 11 / 2, std::rand() % 11 - 11 / 2)) * .2f + prev_move_direction * .8f);
        move_dt(prev_move_direction, dt);
    }


    Player::Player(std::map<std::string, sf::Texture> textures, sf::Vector2f position, int int_ip, int int_local_ip, sf::Uint32 creation_time) : Actor{textures, position}
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


    void Player::updatePosition(sf::Vector2f position)
    {
        auto prev_position = getSprite().getPosition();
        auto move_vector = position - prev_position;
        move(move_vector);
    }


    Player::Player(const Player& player) : Actor{player}
    {
        // Player(std::map<std::string, sf::Texture> textures, sf::Vector2f position, int int_ip, sf::Uint32 creation_time);
        int_ip = player.getIp();
        int_local_ip = player.getLocalIp();
        last_update_time = player.getLastUpdateTime();
    }


    Actor::Actor(const Actor& actor) : Actor{actor.getTextures(), actor.getSprite().getPosition()}
    {
        
    }


    Player::Player() : Actor{}
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
}