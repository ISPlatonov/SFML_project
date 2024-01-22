#include "Actor.hpp"


namespace Actor
{
    Actor::Actor(const sf::Vector2f& position, const std::unordered_map<Object::ObjectName, size_t>& new_inventory)
    {
        if (!Actor::textures.size())
            load_textures("textures/actors/Guy_16x32");
        direction_x = Direction::right;
        sprite.setPosition(position);
        sprite.setTexture(Actor::textures.at(direction_x));
        sprite.setScale(Constants::getPIXEL_SIZE(), Constants::getPIXEL_SIZE());
        setInventory(new_inventory);
    }


    Actor::Actor(sf::Vector2f&& position, std::unordered_map<Object::ObjectName, size_t>&& new_inventory)
    {
        if (!Actor::textures.size())
            load_textures("textures/actors/Guy_16x32");
        direction_x = Direction::right;
        sprite.setPosition(position);
        sprite.setTexture(Actor::textures.at(direction_x));
        sprite.setScale(Constants::getPIXEL_SIZE(), Constants::getPIXEL_SIZE());
        inventory = new_inventory;
    }


    void Actor::check_direction(const sf::Vector2f& direction)
    {
        if (direction.x > 0 && direction_x != Direction::right)
            direction_x = Direction::right;
        else if (direction.x < 0 && direction_x != Direction::left)
            direction_x = Direction::left;
        else
            return;
        sprite.setTexture(Actor::textures.at(direction_x));
    }


    sf::Vector2f Actor::move_dt(const sf::Vector2f& direction, const sf::Uint32& dt, WorldMap::ObjectMap& ObjectMap)
    {
        check_direction(direction); 
        auto v = sf::Vector2f(linalg::normalize(direction) * static_cast<float>(dt) * Constants::getSTEP_SIZE_MULTIPLIER() * static_cast<float>(Constants::getPIXEL_SIZE()));
        const auto& im = ObjectMap.getObjectMap(Object::Passability::impassible);
        for (const auto& object : im)
        {
            object.second.check_collision(v, this->getSprite().getGlobalBounds());
        }
        sprite.move(v);
        return v;
    }


    const size_t Actor::addObject(Object::ObjectName name)
    {
        inventory.count(name) ? ++inventory[name] : inventory[name] = 1;
        return inventory.count(name);
    }


    size_t Actor::removeObject(Object::ObjectName name)
    {
        if (!inventory.count(name))
            return 0;
        --inventory[name];
        auto n = inventory.count(name);
        if (!n)
            inventory.erase(inventory.find(name));
        return n;
    }


    void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        target.draw(sprite, states);
    }


    sf::Vector2f User::move_dt(const sf::Vector2f& direction, const sf::Uint32& dt, WorldMap::ObjectMap& ObjectMap)
    {
        auto v = sf::Vector2f(Actor::move_dt(direction, dt, ObjectMap));
        view.move(v);
        return v;
    }


    User::User(const sf::Vector2f& position, const unsigned int& port, const std::unordered_map<Object::ObjectName, size_t>& new_inventory) : Actor(position, new_inventory)
    {
        // make it static...
        view = sf::View(position + (getSprite().getLocalBounds().getSize() / 2.f * static_cast<float>(Constants::getPIXEL_SIZE())), static_cast<sf::Vector2f>(Constants::getWINDOW_SIZE() * static_cast<float>(Constants::getPIXEL_SIZE())));
        int_ip = sf::IpAddress::getPublicAddress(sf::seconds(5.f)).toInteger();
        int_port = port;
    }


    User::User(sf::Vector2f&& position, unsigned int&& port, std::unordered_map<Object::ObjectName, size_t>&& new_inventory) : Actor(position, new_inventory)
    {
        view = std::move(sf::View(position + (getSprite().getLocalBounds().getSize() / 2.f * static_cast<float>(Constants::getPIXEL_SIZE())), static_cast<sf::Vector2f>(Constants::getWINDOW_SIZE() * static_cast<float>(Constants::getPIXEL_SIZE()))));
        int_ip = std::move(sf::IpAddress::getPublicAddress(sf::seconds(5.f)).toInteger());
        int_port = std::move(port);
    }


    void Bot::make_step(const sf::Uint32& dt, WorldMap::ObjectMap& ObjectMap)
    {
        prev_move_direction = linalg::normalize(linalg::normalize(sf::Vector2f(std::rand() % 11 - 11 / 2, std::rand() % 11 - 11 / 2)) * .2f + prev_move_direction * .8f);
        move_dt(prev_move_direction, dt, ObjectMap);
    }


    void Player::setPosition(const sf::Vector2f& position)
    {
        const auto& prev_position = getPosition();
        auto move_vector = position - prev_position;
        move(move_vector);
    }


    void Player::move(const sf::Vector2f& vector)
    {
        check_direction(vector); 
        sprite.move(vector);
    }


    sf::Packet& operator <<(sf::Packet& packet, const User& user)
    {
        auto position = user.getPosition() / static_cast<float>(Constants::getPIXEL_SIZE());
        Time::Time time = Time::getTimeNow();
        packet << position.x << position.y << Constants::getID() << time;
        packet << static_cast<sf::Uint32>(user.getInventory().size());
        for (const auto& iter : user.getInventory())
        {
            packet << iter.first << static_cast<sf::Uint32>(iter.second);
        }
        return packet;
    }


    void Actor::load_textures(const std::string& texture_dir_path)
    {
        // Load a sprite to display
        auto left_texture = sf::Texture();
        left_texture.loadFromFile(texture_dir_path + "/left.png");
        auto right_texture = sf::Texture();
        right_texture.loadFromFile(texture_dir_path + "/right.png");
        Actor::textures[Direction::left] = std::move(left_texture);
        Actor::textures[Direction::right] = std::move(right_texture);
    }


    // ip and local_ip won't be changed
    Player& operator <<(Player& player, const Multiplayer::PlayerData& player_data)
    {
        player.setPosition(player_data.getPosition() * static_cast<float>(Constants::getPIXEL_SIZE()));
        player.setTime(player_data.getTime());
        player.setInventory(player_data.getInventory());
        return player;
    }
}