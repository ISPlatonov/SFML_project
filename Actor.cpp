#include "Actor.hpp"

Actor::Actor(sf::Sprite sprite)
{
    Actor::sprite = sprite;
    left = 0, right = 0;
    last_action_timepoint = 0;
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
        sprite.move(-float(step_size) * float(dt) * STEP_SIZE_MULTIPLIER, 0);
    else if (!left && right)
        sprite.move(float(step_size) * float(dt) * STEP_SIZE_MULTIPLIER, 0);
    std::cout << float(step_size) * float(dt) * STEP_SIZE_MULTIPLIER << std::endl;
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
