#include "Controls.hpp"


sf::Vector2f Controls::direction;
bool Controls::left = 0,
     Controls::right = 0,
     Controls::up = 0,
     Controls::down = 0,
     Controls::mouse_direction_control = 0,
     Controls::draw_menu = 0,
     Controls::draw_inventory = 0;
sf::RenderWindow Controls::window(sf::VideoMode().getFullscreenModes().at(0), "SFML window", sf::Style::Fullscreen);
sf::RectangleShape Controls::menu = sf::RectangleShape(sf::Vector2f(Controls::window.getSize()) * .1f);
sf::RectangleShape Controls::inventory_rect = sf::RectangleShape(sf::Vector2f(Controls::window.getSize()) * .5f);
WorldMap::ObjectMap Controls::object_map{};
Multiplayer::UdpManager Controls::udp_manager(sf::IpAddress::getLocalAddress(), sf::IpAddress(Constants::getSERVER_IP()));
Actor::User Controls::user(sf::Vector2f(0, 0), Controls::udp_manager.getLocalPort());
std::unordered_map<PlayerId, Actor::Player> Controls::player_pool{};


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


void Controls::handleLeftClick()
{
    if (draw_menu && menu.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition())))
        window.close();
    else if (draw_inventory && inventory_rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition())))
    {
        // select object
        sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition());
        for (auto& [rect, object_name] : inventory)
        {
            if (rect.contains(mouse_pos))
            {
                selected_object = Object::Object(object_name, rect.getPosition() - user.getPosition(), Object::Passability::foreground);
                break;
            }
        }
    }
    else
    {
        // handle object selection
        for (const auto& iter : object_map.getObjectMap(Object::Passability::foreground))
        {
            if (iter.second.getSprite().getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition())))
            {
                sf::Packet data;
                data << Multiplayer::DataType::Event;
                data << Multiplayer::EventType::takeObjectToInventory;
                sf::Vector2f object_position(iter.second.getPosition().x / Constants::getPIXEL_SIZE(), iter.second.getPosition().y / Constants::getPIXEL_SIZE());
                auto object_data = Multiplayer::ObjectData(object_position, Time::getTimeNow(), SocketInfo(0, 0) /* ??? */, iter.second.getName(), iter.second.getPassability());
                data << object_data;
                data << user;
                udp_manager.send(data);
                break;
            }
        }
    }
}


void Controls::handleMouseDirection()
{
    const auto& mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition());
    direction = mouse_pos - user.getPosition() - user.getSprite().getGlobalBounds().getSize() / 2.f;
}


void Controls::addEvent(const sf::Event& event)
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
                case (sf::Keyboard::Tab):
                    draw_inventory = !draw_inventory;
                    selected_object = Object::Object();
                    break;
                case (sf::Keyboard::F):
                    handleLeftClick();
                    break;
                case (sf::Keyboard::LShift):
                    mouse_direction_control = 1;
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
                case (sf::Keyboard::LShift):
                    mouse_direction_control = 0;
                    break;
                default:
                    break;
            }
            break;
        case sf::Event::MouseButtonPressed:
            switch (event.mouseButton.button)
			{
				case sf::Mouse::Left:
					handleLeftClick();
					break;
				case sf::Mouse::Right:
                    mouse_direction_control = 1;
					break;
				default:
					break;
			}
            break;
        case sf::Event::MouseButtonReleased:
            switch (event.mouseButton.button)
            {
                case sf::Mouse::Right:
                    mouse_direction_control = 0;
					break;
				default:
					break;
            }
            break;
        default:
            break;
    }
}


void Controls::drawInterfaces()
{
    if (draw_inventory)
    {
        inventory.clear();
        const auto& center = user.getView().getCenter();
        auto text = sf::Text("Inventory", font);
        text.setFillColor(sf::Color::White);
        inventory_rect.setPosition(center - inventory_rect.getSize() / 2.f);
        text.setPosition(center - inventory_rect.getSize() / 2.f);
        inventory_rect.setOutlineColor(sf::Color::Cyan);
        inventory_rect.setOutlineThickness(static_cast<float>(Constants::getPIXEL_SIZE()) / 2.f);
        inventory_rect.setFillColor(sf::Color(0, 0, 0, 127));
        
        window.draw(inventory_rect);
        window.draw(text);
        sf::Vector2f shift(5, 5);
        shift *= static_cast<float>(Constants::getPIXEL_SIZE());
        shift.y += text.getGlobalBounds().getSize().y;
        for (const auto& iter : user.getInventory())
        {
            if (!Object::Object::NameToTextureMap.count(iter.first))
            {
                auto new_texture = sf::Texture();
                new_texture.loadFromFile(Object::Object::NameToTextureAddressMap.at(iter.first));
                Object::Object::NameToTextureMap[iter.first] = std::move(new_texture);
            }
            auto sprite = sf::Sprite(Object::Object::NameToTextureMap[iter.first]);
            sprite.setScale(Constants::getPIXEL_SIZE(), Constants::getPIXEL_SIZE());
            sprite.setPosition(inventory_rect.getGlobalBounds().getPosition() + shift);
            inventory[sprite.getGlobalBounds()] = iter.first;
            auto text = sf::Text(std::to_string(iter.second), font);
            text.setFillColor(sf::Color::White);
            text.setPosition(inventory_rect.getGlobalBounds().getPosition() + shift);
            shift += sf::Vector2f(17, 0) * static_cast<float>(Constants::getPIXEL_SIZE());
            window.draw(sprite);
            window.draw(text);
        }
        if (selected_object.getPassability() == Object::Passability::foreground)
        {
            // draw rectangle around selected object
            // and set position related on window bounds
            auto rect = sf::RectangleShape(selected_object.getSprite().getGlobalBounds().getSize() + sf::Vector2f(2, 2) * static_cast<float>(Constants::getPIXEL_SIZE()));
            rect.setFillColor(sf::Color(0, 0, 0, 0));
            auto color = sf::Color::Green;
            color.a /= 2;
            rect.setOutlineColor(color);
            rect.setOutlineThickness(static_cast<float>(Constants::getPIXEL_SIZE()) / 2.f);
            rect.setPosition(selected_object.getSprite().getGlobalBounds().getPosition() + user.getPosition() - sf::Vector2f(1, 1) * static_cast<float>(Constants::getPIXEL_SIZE()));
            window.draw(rect);
        }
    }
    if (draw_menu)
    {
        auto center = user.getView().getCenter();
        auto text = sf::Text("Exit", font);
        text.setFillColor(sf::Color::White);
        menu.setPosition(center - menu.getSize() / 2.f);
        text.setPosition(center - text.getGlobalBounds().getSize() / 2.f);
        menu.setOutlineColor(sf::Color::Cyan);
        menu.setOutlineThickness(static_cast<float>(Constants::getPIXEL_SIZE()) / 2.f);
        menu.setFillColor(sf::Color(0, 0, 0, 127));
        
        window.draw(menu);
        window.draw(text);
    }
}


sf::Vector2f Controls::getDirection()
{
    handleMouseDirection();
    return linalg::normalize(!mouse_direction_control ? sf::Vector2f((right ? 1.f : 0.f) - (left ? 1.f : 0.f), (down ? 1.f : 0.f) - (up ? 1.f : 0.f)) : direction);
}


void Controls::handleFrameStep()
{
    // needs another thread
    // receiving
    size_t rest = Constants::getUDP_PACKETS_GAP() + udp_manager.getPlayerDataPool().size() << 1 + 1;
    while (udp_manager.receive() == sf::Socket::Done && rest--)
        continue;
    // handling player_data_pool
    for (const auto& iter : udp_manager.getPlayerDataPool())
    {
        if (iter.first == Constants::getID())
            continue;
        auto time = iter.second.getTime();
        Time::Time time_now = Time::getTimeNow();
        int ping = static_cast<int>(time_now) - static_cast<int>(time);
        if (!player_pool.count(iter.first))
        {
            player_pool[iter.first] = Actor::Player(iter.second);
            continue;
        }
        else if (ping > Constants::getMAX_PING())
        {
            player_pool.erase(iter.first);
            //udp_manager.removePlayerByPlayerId(iter.second.getId());
            continue;
        }
        else
        {
            player_pool[iter.first] << iter.second;
            continue;
        }
    }
    for (auto& pair : udp_manager.getObjectDataPool())
        for (auto& obj : pair.second)
            object_map.addObject(obj);
    Controls::udp_manager.clearObjectDataPool();
    for (auto object_data = udp_manager.getRemovedObjectData(); object_data.getTime() != 0; object_data = udp_manager.getRemovedObjectData())
        Controls::object_map.removeObject(object_data);
    for (auto object_data = udp_manager.getObjectToInventoryData(); object_data.getTime() != 0; object_data = udp_manager.getObjectToInventoryData())
        Controls::user.addObject(object_data.getName());
    sf::Packet data;
    data << Multiplayer::DataType::Player << Controls::user;
    udp_manager.send(data);
    handleEvents();
    auto norm_direction = Controls::getDirection();
    auto dt = Controls::getDeltaTime();
    Controls::user.move_dt(norm_direction, dt, Controls::object_map);
    Controls::setLastActionTimepoint();
    // Clear screen
    window.clear();
    // Draw the sprite
    //window.draw(WorldMap::WorldMap::map);
    auto& bm = object_map.getObjectMap(Object::Passability::background);
    for (auto iter = bm.begin(); iter != bm.end();) {
        const auto& position = iter->second.getPosition();
        const auto& it_size = iter->second.getSprite().getGlobalBounds().getSize();
        const auto& center = window.getView().getCenter();
        const auto& size = window.getView().getSize();
        if (position.x + it_size.x < center.x - size.x / 2.f ||
            position.x > center.x + size.x / 2.f ||
            position.y + it_size.y < center.y - size.y / 2.f ||
            position.y > center.y + size.y / 2.f) {
                iter = bm.erase(iter);
                continue;
            }
            
        window.draw(iter->second);
        ++iter;
    }
    auto& im = object_map.getObjectMap(Object::Passability::impassible);
    for (auto iter = im.begin(); iter != im.end();) {
        const auto& position = iter->second.getPosition();
        const auto& it_size = iter->second.getSprite().getGlobalBounds().getSize();
        const auto& center = window.getView().getCenter();
        const auto& size = window.getView().getSize();
        if (position.x + it_size.x < center.x - size.x / 2.f ||
            position.x > center.x + size.x / 2.f ||
            position.y + it_size.y < center.y - size.y / 2.f ||
            position.y > center.y + size.y / 2.f) {
                iter = im.erase(iter);
                continue;
            }
            
        window.draw(iter->second);
        ++iter;
    }
    for (const auto& player : player_pool)
        if (player.second.getLastUpdateTime() + Constants::getMAX_PING() >= Time::getTimeNow())
            Controls::window.draw(player.second.getSprite());
    Controls::window.draw(user.getSprite());
    auto& fm = object_map.getObjectMap(Object::Passability::foreground);
    for (auto iter = fm.begin(); iter != fm.end();) {
        const auto& position = iter->second.getPosition();
        const auto& it_size = iter->second.getSprite().getGlobalBounds().getSize();
        const auto& center = window.getView().getCenter();
        const auto& size = window.getView().getSize();
        if (position.x + it_size.x < center.x - size.x / 2.f ||
            position.x > center.x + size.x / 2.f ||
            position.y + it_size.y < center.y - size.y / 2.f ||
            position.y > center.y + size.y / 2.f) {
                iter = fm.erase(iter);
                continue;
            }
            
        window.draw(iter->second);
        ++iter;
    }
    {
        // draw border around object that curson is on
        auto& mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition());
        for (const auto& iter : object_map.getObjectMap(Object::Passability::foreground))
		{
			if (iter.second.getSprite().getGlobalBounds().contains(mouse_pos))
			{
				auto rect = sf::RectangleShape(iter.second.getSprite().getGlobalBounds().getSize() + sf::Vector2f(2, 2) * static_cast<float>(Constants::getPIXEL_SIZE()));
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                auto color = sf::Color::Green;
                color.a /= 2;
                rect.setOutlineColor(color);
                rect.setOutlineThickness(static_cast<float>(Constants::getPIXEL_SIZE()) / 2.f);
                rect.setPosition(iter.second.getSprite().getGlobalBounds().getPosition() - sf::Vector2f(1, 1) * static_cast<float>(Constants::getPIXEL_SIZE()));
                window.draw(rect);
				break;
			}
		}
	}
    {
        const auto& bm = object_map.getObjectMap(Object::Passability::background);
        const auto& im = object_map.getObjectMap(Object::Passability::impassible);
        const auto& fm = object_map.getObjectMap(Object::Passability::foreground);
        const auto& odp = udp_manager.getObjectDataPool();
        auto text = sf::Text(std::to_string(bm.size()) + " " + std::to_string(im.size()) + " " + std::to_string(fm.size()) + " " + std::to_string(odp.size()), font);
        text.setFillColor(sf::Color::White);
        text.setPosition(window.getView().getCenter() - window.getView().getSize() / 2.f);
        window.draw(text);
    }
    // Draw the string
    //window.draw(text);
    window.setView(user.getView());
    drawInterfaces();
    // Update the window
    window.display();
}