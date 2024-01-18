#pragma once

#include <SFML/System.hpp>
#include <chrono>


namespace Time {
    typedef sf::Uint32 Time;
    
    inline Time getTimeNow() { return static_cast<Time>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()); }
}