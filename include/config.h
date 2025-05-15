#pragma once

#include <SFML/Graphics.hpp>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
sf::Vector2i const SCREEN_DIM(SCREEN_WIDTH, SCREEN_HEIGHT); // px
sf::Vector2u const GRID_DIM(BOARD_ROWS, BOARD_COLS);        // cells
constexpr int TARGET_FPS = 60;

constexpr unsigned BOARD_ROWS = 5;
constexpr unsigned BOARD_COLS = 5;
constexpr unsigned TILE_SIZE_PX = 100;
