#include "../include/board.h"
#include "../include/turret.h"
#include "../include/util.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr unsigned BOARD_ROWS = 5;
constexpr unsigned BOARD_COLS = 5;
constexpr unsigned TILE_SIZE_PX = 100; // height gets halved in iso-space

// LLM code, probably should just store grid-space and iso-space coords in tile
bool point_in_diamond(const sf::ConvexShape &shape, const sf::Vector2f &point) {
  // Assumes diamond has 4 points: top, right, bottom, left
  if (shape.getPointCount() != 4)
    return false;

  // Get transformed points
  sf::Transform transform = shape.getTransform();
  sf::Vector2f top = transform.transformPoint(shape.getPoint(0));
  sf::Vector2f right = transform.transformPoint(shape.getPoint(1));
  sf::Vector2f bottom = transform.transformPoint(shape.getPoint(2));
  sf::Vector2f left = transform.transformPoint(shape.getPoint(3));

  // Compute center (intersection of diagonals)
  sf::Vector2f center = {(left.x + right.x) * 0.5f, (top.y + bottom.y) * 0.5f};

  // Compute width and height (distance between opposite corners)
  float width = std::abs(right.x - left.x);
  float height = std::abs(bottom.y - top.y);

  float dx = std::abs(point.x - center.x);
  float dy = std::abs(point.y - center.y);

  return (dx / (width * 0.5f) + dy / (height * 0.5f)) <= 1.0f;
}

int main() {
  const sf::Vector2i screen_dim(SCREEN_WIDTH, SCREEN_HEIGHT);
  const sf::Vector2u grid_dim(BOARD_ROWS, BOARD_COLS);

  // SETUP
  sf::RenderWindow window;
  setup_window(window, screen_dim);
  Board board(grid_dim, TILE_SIZE_PX, screen_dim);

  std::vector<Turret> turrets;


  for (auto &tile : board.m_tiles) {
    turrets.emplace_back(Turret(tile.m_shape.getPosition(), TILE_SIZE_PX));
  }


  float angle = 0.f;
  float ellipse_width = 90.f;
  float ellipse_height = 45.f;


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
      if (point_in_diamond(tile.m_shape, sf::Vector2f(mouse_pos))) {
        tile.m_shape.setFillColor(sf::Color(100, 100, 100));
      } else {
        tile.m_shape.setFillColor(sf::Color(0, 0, 0));
      }
    }

    angle += .005;
    for (auto &turret : turrets) {
      const float a = ellipse_width / 2.f;
      const float b = ellipse_height / 2.f;
      const float x = turret.barrel_anchor.x + a * std::cos(angle);
      const float y = turret.barrel_anchor.y + b * std::sin(angle);
      turret.barrel_shape.setPosition(x, y);
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
