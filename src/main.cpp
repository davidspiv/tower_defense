#include "../include/Board.h"
#include "../include/Game_State.h"
#include "../include/Tower.h"
#include "../include/util.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread> // For std::this_thread::sleep_for
#include <vector>


int main() {
  constexpr int SCREEN_WIDTH = 1920;
  constexpr int SCREEN_HEIGHT = 1080;
  constexpr unsigned BOARD_ROWS = 5;
  constexpr unsigned BOARD_COLS = 5;
  constexpr unsigned TILE_SIZE_PX = 100;
  constexpr int TARGET_FPS = 60;

  const sf::Vector2i SCREEN_DIM(SCREEN_WIDTH, SCREEN_HEIGHT); // px
  const sf::Vector2u GRID_DIM(BOARD_ROWS, BOARD_COLS);        // cells

  Game_State game(SCREEN_DIM, GRID_DIM, TILE_SIZE_PX);

  sf::RenderWindow window;
  setup_window(window, SCREEN_DIM);

  game.run(window, TARGET_FPS);
}
