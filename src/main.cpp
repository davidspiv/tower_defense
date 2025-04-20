#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include "../include/board.h"
#include "../include/util.h"

int main() {
  sf::Vector2i screen_size(1920, 1080);

  // WINDOW SETUP
  sf::RenderWindow window;
  setup_window(window, screen_size);

  // BOARD
  const float rows = 16;
  const float cols = 16;
  const float tile_size = 16;

  Board board(rows, cols, tile_size, screen_size);

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

    board.draw(window);

    window.display();  // Show what was drawn
  }
}
