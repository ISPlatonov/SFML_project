#pragma once

#define PIXEL_SIZE size_t(10)
#define WINDOW_SIZE_X size_t(256)
#define WINDOW_SIZE_Y size_t(128)
#define WINDOW_SIZE sf::Vector2u(WINDOW_SIZE_X, WINDOW_SIZE_Y)
//#define WORK_SPACE sf::Rect(16, 8, 96, 48)
#define FRAMERATE_LIMIT size_t(60)
#define STEP_SIZE_MULTIPLIER float(.05)

#define TILE_SIZE /*size_t(8)*/ PIXEL_SIZE