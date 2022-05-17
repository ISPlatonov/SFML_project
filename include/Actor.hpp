#pragma once

#include <SFML/Graphics.hpp>
#include <chrono>
#include <map>
#include <string>
#include "Constants.hpp"
#include "LinearAlgebra.hpp"


class Actor : sf::Drawable, sf::Transformable
{
public:
    Actor(std::map<std::string, sf::Texture> textures, sf::Vector2f position);
    void move_dt(sf::Vector2f direction, intmax_t dt);
    //void setPosition(sf::Vector2f p);
    void check_direction(sf::Vector2f);

    const sf::Sprite getSprite();

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
    void make_step(intmax_t dt);

private:
    sf::Vector2f prev_move_direction = linalg::normalize(sf::Vector2f(std::rand() % 10 - 10 / 2, std::rand() % 10 - 10 / 2));
};


class User : public Actor
{
public:
    User(std::map<std::string, sf::Texture> textures, sf::Vector2f position);
    void move_dt(sf::Vector2f direction, intmax_t dt);
    const sf::View& getView();
    
private:
    sf::View view;
};