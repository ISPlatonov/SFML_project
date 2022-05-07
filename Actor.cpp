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
    /*
    left = 0, 
    right = 0, 
    up = 0, 
    down = 0;
    last_action_timepoint = 0;
    */

    view = sf::View(position + (getSprite().getLocalBounds().getSize() / 2.f), static_cast<sf::Vector2f>(WINDOW_SIZE * static_cast<unsigned int>(PIXEL_SIZE)));
};

// need a fix!
void Actor::setPosition(sf::Vector2f p)
{
    sprite.setPosition(p);
};
/*
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

void Actor::PressedW()
{
    if (!up)
    {
        up = 1;
        last_action_timepoint = std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

void Actor::PressedS()
{
    if (!down)
    {
        down = 1;
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

void Actor::ReleasedW()
{
    if (up)
    {
        up = 0;
        last_action_timepoint = std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

void Actor::ReleasedS()
{
    if (down)
    {
        down = 0;
        last_action_timepoint = std::chrono::steady_clock::now().time_since_epoch().count();
    }
};

template <typename T>
void Actor::step(T step_size)
{
    if (((left && right) || (!left && !right)) && ((up && down) || (!up && !down)))
        return;
    intmax_t t = std::chrono::steady_clock::now().time_since_epoch().count();
    intmax_t dt = t - last_action_timepoint;

    auto move_size = float(step_size) * float(PIXEL_SIZE) * float(dt) * STEP_SIZE_MULTIPLIER;

    if (left && !right)
    {
        sprite.setTexture(textures["left"]);
        sprite.move(-move_size, 0);
        view.move(-move_size, 0);
    }
    else if (!left && right)
    {
        sprite.setTexture(textures["right"]);
        sprite.move(move_size, 0);
        view.move(move_size, 0);
    }

    if (up && !down)
    {
        //sprite.setTexture(textures["left"]);
        sprite.move(0, -move_size);
        view.move(0, -move_size);
    }
    else if (!up && down)
    {
        //sprite.setTexture(textures["right"]);
        sprite.move(0, move_size);
        view.move(0, move_size);
    }  

    last_action_timepoint = t;
};

void Actor::step()
{
    step(1);
};
*/

const sf::Sprite Actor::getSprite()
{
    return sprite;
};

void Actor::move(sf::Vector2f direction, intmax_t dt)
{
    auto v = direction * static_cast<float>(dt) * STEP_SIZE_MULTIPLIER * static_cast<float>(PIXEL_SIZE);
    sprite.move(v);
    view.move(v);
};