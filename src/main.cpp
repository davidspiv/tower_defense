#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include "../include/board.h"
#include "../include/util.h"

int main() {
  const unsigned screen_width = 1920;
  const unsigned screen_height = 1080;

  // WINDOW SETUP
  sf::RenderWindow window;
  setup_window(window, screen_width, screen_height);

  // BOARD
  const float rows = 16;
  const float cols = 16;
  const float tile_size = 16;

  Board board(rows, cols, tile_size, screen_width, screen_height);

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

    for (auto& tile : board.tiles) {
      window.draw(tile.shape);
    }

    window.display();  // Show what was drawn
  }
}
