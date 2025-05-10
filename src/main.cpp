#include "../include/Board.h"
#include "../include/Engine.h"
#include "../include/Tower.h"
#include "../include/config.h"
#include "../include/util.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread> // For std::this_thread::sleep_for
#include <vector>


int main() {
  const sf::Vector2i screen_dim(SCREEN_WIDTH, SCREEN_HEIGHT); // px
  const sf::Vector2u grid_dim(BOARD_ROWS, BOARD_COLS);        // cells

  Engine game(screen_dim, grid_dim, TILE_SIZE_PX);

  game.run(TARGET_FPS);
}
