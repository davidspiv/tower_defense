#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include "../include/board.h"
#include "../include/util.h"

int main() {
  const unsigned w_width = 1920;
  const unsigned w_height = 1080;

  // WINDOW SETUP
  sf::RenderWindow window;
  setup_window(window, w_width, w_height);

  // BOARD
  const float tile_size = 100;
  const Board board(w_width, w_height, tile_size);
  const sf::VertexArray grid = build_grid(board);
  std::vector<Tile> tiles = get_tiles(board);

  // GAMEPLAY LOOP
  while (window.isOpen()) {
    /*
    ****************************************
    Handle the players input
    ****************************************
    */
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }

    /*
    ****************************************
    Draw the scene
    ****************************************
    */
    window.clear();

    for (auto& tile : tiles) {
      window.draw(tile.shape);
    }

    // window.draw(grid);

    window.display();  // Show what was drawn
  }
}
