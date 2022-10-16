#include "Controls.hpp"


sf::Vector2f Controls::direction;
bool Controls::left = 0,
     Controls::right = 0,
     Controls::up = 0,
     Controls::down = 0,
     Controls::draw_menu = 0;
sf::Uint32 Controls::last_action_timepoint;
sf::RenderWindow Controls::window(sf::VideoMode().getFullscreenModes().at(0), "SFML window", sf::Style::Fullscreen);
Actor::User Controls::user(sf::Vector2f(0, 0));
sf::RectangleShape Controls::menu = sf::RectangleShape(sf::Vector2f(Controls::window.getSize()) * .1f);
WorldMap::ObjectMap Controls::object_map{};
Multiplayer::UdpManager Controls::udp_manager(sf::IpAddress::getLocalAddress(), sf::IpAddress(Constants::getSERVER_IP()));
std::map<std::string, Actor::Player> Controls::player_pool{};
sf::Font Controls::font;


void Controls::applyWindowSettings()
{
    window.setFramerateLimit(Constants::getFRAMERATE_LIMIT());
    window.setVerticalSyncEnabled(Constants::getENABLE_VSYNC());
    // load font
    font.loadFromFile("textures/fonts/font.ttf");
}


void Controls::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
        addEvent(event);
}


void Controls::addEvent(sf::Event event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
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
            break;
        case sf::Event::KeyReleased:
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
            break;
        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Left:
                    if (draw_menu && menu.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition())))
                        window.close();
                    else
                    {
                        // handle object selection
                        for (auto iter : object_map.getObjectMap(Object::Passability::foreground))
                        {
                            if (iter.second.getSprite().getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition())))
                            {
                                sf::Packet data;
                                data << Multiplayer::DataType::Event;
                                data << Multiplayer::EventType::takeObjectToInventory;
                                data << iter.second;
                                data << user;
                                udp_manager.send(data);
                                break;
                            }
                        }
                    }
                    break;
            }
            break;
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
        auto center = user.getView().getCenter();
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


void Controls::handleFrameStep()
{
    // needs another thread
    // receiving
    for (size_t i = 0; i < Controls::player_pool.size() + Constants::getUDP_PACKETS_GAP(); ++i)
    {
        udp_manager.receive();
    }
    // handling player_data_pool
    for (auto iter = udp_manager.getPlayerDataPool().begin(); iter != udp_manager.getPlayerDataPool().end();)
    {
        auto time = (*iter).second.getTime();
        sf::Uint32 time_now = static_cast<sf::Uint32>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
        int ping = static_cast<int>(time_now) - static_cast<int>(time);
        if (!player_pool.count((*iter).first))
        {
            player_pool[(*iter).first] = Actor::Player((*iter).second);
            ++iter;
            continue;
        }
        else if (ping > Constants::getMAX_PING())
        {
            player_pool.erase((*iter).first);
            udp_manager.removePlayerById((*iter++).first);
            continue;
        }
        else
        {
            player_pool[(*iter).first] << (*iter).second;
            ++iter;
            continue;
        }
    }
    for (auto iter = udp_manager.getObjectDataPool().begin(); iter != udp_manager.getObjectDataPool().end(); ++iter)
        Controls::object_map.addObject((*iter).second);
    for (auto object_data = udp_manager.getRemovedObjectData(); object_data.getTime() != 0; object_data = udp_manager.getRemovedObjectData())
        Controls::object_map.removeObject(object_data);
    sf::Packet data;
    data << Multiplayer::DataType::Player << Controls::user;
    udp_manager.send(data);
    handleEvents();
    auto direction = Controls::getDirection();
    auto dt = Controls::getDeltaTime();
    Controls::user.move_dt(direction, dt, Controls::object_map);
    Controls::setLastActionTimepoint();
     // Clear screen
    window.clear();
    // Draw the sprite
    window.draw(WorldMap::WorldMap::map);
    for (auto iter : object_map.getObjectMap(Object::Passability::background))
        window.draw(iter.second);
    for (auto iter : object_map.getObjectMap(Object::Passability::impassible))
        Controls::window.draw(iter.second);
    for (const auto& player : player_pool)
        Controls::window.draw(player.second.getSprite());
    Controls::window.draw(user.getSprite());
    for (auto iter : object_map.getObjectMap(Object::Passability::foreground))
        window.draw(iter.second);
    // Draw the string
    //window.draw(text);
    window.setView(user.getView());
    drawMenu();
    // Update the window
    window.display();
}