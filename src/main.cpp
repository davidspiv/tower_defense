#include "../include/board.h"
#include "../include/bullet.h"
#include "../include/enemy.h"
#include "../include/turret.h"
#include "../include/util.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr unsigned BOARD_ROWS = 5;
constexpr unsigned BOARD_COLS = 5;
constexpr unsigned TILE_SIZE_PX = 100;

int main() {
  const sf::Vector2i screen_dim(SCREEN_WIDTH, SCREEN_HEIGHT);
  const sf::Vector2u grid_dim(BOARD_ROWS, BOARD_COLS);

  // SETUP
  sf::RenderWindow window;
  setup_window(window, screen_dim);
  Board board(grid_dim, TILE_SIZE_PX, screen_dim);

  std::vector<Enemy> enemies;
  std::vector<Bullet> bullets;
  std::vector<Turret> turrets;

  for (auto &tile : board.m_tiles) {
    turrets.emplace_back(Turret(tile.m_shape.getPosition(), TILE_SIZE_PX));
  }

  // GAMEPLAY LOOP
  while (window.isOpen()) {
    // INPUT
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }

    const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

    // UPDATE
    for (auto &tile : board.m_tiles) {
      if (board.point_in_iso_tile(sf::Vector2f(mouse_pos), tile,
                                  board.m_tile_size)) {
        tile.m_shape.setFillColor(sf::Color(100, 100, 100));
      } else {
        tile.m_shape.setFillColor(sf::Color(0, 0, 0));
      }
    }

    for (auto &turret : turrets) {
      turret.update();
    }

    // DRAW
    window.clear();
    board.draw(window);

    for (auto &turret : turrets) {
      if (turret.barrel_shape.getPosition().y > turret.barrel_anchor.y) {
        window.draw(turret.base_shape);
        window.draw(turret.barrel_shape);
      } else {
        window.draw(turret.barrel_shape);
        window.draw(turret.base_shape);
      }
    }

    window.display();
  }
}
