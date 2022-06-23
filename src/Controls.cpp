#include "Controls.hpp"


sf::Vector2f Controls::direction;
bool Controls::left = 0,
     Controls::right = 0,
     Controls::up = 0,
     Controls::down = 0,
     Controls::draw_menu = 0;
sf::Uint32 Controls::last_action_timepoint;
sf::RenderWindow Controls::window(sf::VideoMode().getFullscreenModes().at(0), "SFML window", sf::Style::Fullscreen);
Actor::User Controls::user(Actor::load_textures("textures/actors/Guy_16x32"), - sf::Vector2f(16, 32) / 2.f * static_cast<float>(PIXEL_SIZE));
sf::RectangleShape Controls::menu = sf::RectangleShape(sf::Vector2f(Controls::window.getSize()) * .1f);


void Controls::applyWindowSettings()
{
    window.setFramerateLimit(FRAMERATE_LIMIT);
    //window.setVerticalSyncEnabled(VSYNC);
}


void Controls::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
            window.close();
        addEvent(event);
    }
}


void Controls::addEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
        switch (event.key.code)
        {
            case (sf::Keyboard::A):
                left = 1;
                break;
            case (sf::Keyboard::D):
                right = 1;
                break;
            case (sf::Keyboard::W):
                up = 1;
                break;
            case (sf::Keyboard::S):
                down = 1;
                break;
            case (sf::Keyboard::Escape):
                // open menu
                draw_menu = !draw_menu;
                break;
            default:
                break;
        }
    else if (event.type == sf::Event::KeyReleased)
        switch (event.key.code)
        {
            case (sf::Keyboard::A):
                left = 0;
                break;
            case (sf::Keyboard::D):
                right = 0;
                break;
            case (sf::Keyboard::W):
                up = 0;
                break;
            case (sf::Keyboard::S):
                down = 0;
                break;
            default:
                break;
        }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            if (draw_menu && menu.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition())))
                window.close();
        }
}


sf::Vector2f Controls::getDirection()
{
    float x, y;

    x = right * 1.f - left * 1.f;
    y = down * 1.f - up * 1.f;

    auto vector = sf::Vector2f(x, y);
    linalg::normalize(vector);

    // rewrite!
    return vector;
}


void Controls::setLastActionTimepoint(sf::Uint32 t)
{
    last_action_timepoint = t;
}


sf::Uint32 Controls::getDeltaTime()
{
    sf::Uint32 t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    sf::Uint32 dt = t - last_action_timepoint;
    //setLastActionTimepoint(t);

    return dt;
}


void Controls::drawMenu()
{
    if (draw_menu)
    {
        auto center = user.getView().getCenter() + static_cast<sf::Vector2f>(user.getSprite().getTexture()->getSize()) / 2.f * static_cast<float>(PIXEL_SIZE);
        auto font = sf::Font();
        font.loadFromFile("textures/font.ttf");
        auto text = sf::Text("Exit", font);
        text.setFillColor(sf::Color::Black);
        menu.setPosition(center - menu.getSize() / 2.f);
        text.setPosition(center - menu.getSize() / 2.f);
        menu.setOutlineColor(sf::Color::Cyan);
        menu.setOutlineThickness(5);
        
        window.draw(menu);
        window.draw(text);
    }
}