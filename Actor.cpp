#include "Actor.hpp"

/*Actor::Actor(sf::Texture texture_left, sf::Texture texture_right)
{
    textures["left"] = texture_left;
    textures["right"] = texture_right;
    sprite.setTexture(textures["right"]);
    sprite.setScale(PIXEL_SIZE, PIXEL_SIZE);
    left = 0, right = 0;
    last_action_timepoint = 0;
};*/

Actor::Actor(std::map<std::string, sf::Texture> textures, sf::Vector2f position)
{
    Actor::textures = textures;
    setPosition(position);
    sprite.setTexture(Actor::textures["right"]);
    sprite.setScale(PIXEL_SIZE, PIXEL_SIZE);
    left = 0, right = 0;
    last_action_timepoint = 0;
};

void Actor::setPosition(sf::Vector2f p)
{
    sprite.setPosition(p);
};

void Actor::PressedA()
{
    if (!left)
    {
        left = 1;
        last_action_timepoint = std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

void Actor::PressedD()
{
    if (!right)
    {
        right = 1;
        last_action_timepoint = std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

void Actor::ReleasedA()
{
    if (left)
    {
        left = 0;
        last_action_timepoint = std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

void Actor::ReleasedD()
{
    if (right)
    {
        right = 0;
        last_action_timepoint = std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

template <typename T>
void Actor::step(T step_size)
{
    if ((left && right) || (!left && !right))
        return;
    intmax_t t = std::chrono::steady_clock::now().time_since_epoch().count();
    intmax_t dt = t - last_action_timepoint;
    if (left && !right)
    {
        sprite.setTexture(textures["left"]);
        sprite.move(-float(step_size) * float(dt) * STEP_SIZE_MULTIPLIER, 0);
    }
    else if (!left && right)
    {
        sprite.setTexture(textures["right"]);
        sprite.move(float(step_size) * float(dt) * STEP_SIZE_MULTIPLIER, 0);
    }
    last_action_timepoint = t;
};

void Actor::step()
{
    step(1);
};

const sf::Sprite Actor::getSprite()
{
    return sprite;
};
