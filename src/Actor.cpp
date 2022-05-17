#include "Actor.hpp"


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


const sf::Sprite Actor::getSprite()
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


void Actor::move_dt(sf::Vector2f direction, intmax_t dt)
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


void User::move_dt(sf::Vector2f direction, intmax_t dt)
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
}


const sf::View& User::getView()
{
    return view;
}


void Bot::make_step(intmax_t dt)
{
    prev_move_direction = linalg::normalize(sf::Vector2f(std::rand() % 11 - 11 / 2, std::rand() % 11 - 11 / 2)) * .05f + prev_move_direction * .95f;
    move_dt(prev_move_direction, dt);
}