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
bool point_in_shape(const sf::ConvexShape &shape, const sf::Vector2f &point) {
  // Transform the shape points to global coordinates
  sf::Transform transform = shape.getTransform();
  std::vector<sf::Vector2f> points;

  for (size_t i = 0; i < shape.getPointCount(); ++i) {
    points.push_back(transform.transformPoint(shape.getPoint(i)));
  }

  // Ray-casting algorithm to determine if point is inside polygon
  bool inside = false;
  size_t j = points.size() - 1;
  for (size_t i = 0; i < points.size(); ++i) {
    const sf::Vector2f &pi = points[i];
    const sf::Vector2f &pj = points[j];

    if (((pi.y > point.y) != (pj.y > point.y)) &&
        (point.x < (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x)) {
      inside = !inside;
    }

    j = i;
  }

  return inside;
}


int main() {
  const sf::Vector2i screen_dimensions(SCREEN_WIDTH, SCREEN_HEIGHT);
  const sf::Vector2u grid_dimensions(BOARD_ROWS, BOARD_COLS);

  // SETUP
  sf::RenderWindow window;
  setup_window(window, screen_dimensions);
  Board board(grid_dimensions, TILE_SIZE_PX, screen_dimensions);

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
      if (point_in_shape(tile.m_shape, sf::Vector2f(mouse_pos))) {
        tile.m_shape.setFillColor(sf::Color(100, 100, 100));
      } else {
        tile.m_shape.setFillColor(sf::Color(0, 0, 0));
      }
    }

    angle += .005;
    for (auto &turret : turrets) {
      const float a = ellipse_width / 2.f;
      const float b = ellipse_height / 2.f;
      const float x = turret.barrel_ellipse_center.x + a * std::cos(angle);
      const float y = turret.barrel_ellipse_center.y + b * std::sin(angle);
      turret.barrel_shape.setPosition(x, y);
    }


    // DRAW
    window.clear();
    board.draw(window);


    for (auto &turret : turrets) {
      if (turret.barrel_shape.getPosition().y >
          turret.barrel_ellipse_center.y) {
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
