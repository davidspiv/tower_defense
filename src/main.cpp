#include <SFML/Graphics.hpp>
#include <vector>

#include "../include/board.h"
#include "../include/util.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BOARD_ROWS 16
#define BOARD_COLS 16
#define TILE_SIZE_PX 32 // height gets halved in iso-space, width stays constant

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

sf::VertexArray get_turret() {
  sf::VertexArray blob(sf::TriangleFan);
  // Center point (approximately the center of the shape)
  blob.append(sf::Vertex({101.0f, 149.5f}, sf::Color::White)); // index 0

  const static std::vector<sf::Vector2f> points = {
      {103.0f, 0.0f},   // Top center
      {140.0f, 5.0f},   // Upper-right curve
      {180.0f, 60.0f},  // Right middle bulge
      {202.0f, 130.0f}, // Right top third
      {202.0f, 252.0f}, // Right bottom
      {180.0f, 280.0f}, // Bottom-right curve
      {140.0f, 294.0f}, // Bottom center right
      {103.0f, 299.0f}, // Bottom center
      {66.0f, 294.0f},  // Bottom center left
      {26.0f, 280.0f},  // Bottom-left curve
      {0.0f, 252.0f},   // Left bottom
      {0.0f, 130.0f},   // Left top third
      {22.0f, 60.0f},   // Left middle bulge
      {66.0f, 5.0f},    // Upper-left curve
      {99.0f, 0.0f}     // Back to near start
  };

  for (auto point : points) {
    blob.append(sf::Vertex(point, sf::Color::White));
  }

  // Closing loop
  blob.append(sf::Vertex(points[1], sf::Color::White));

  return blob;
}

int main() {
  sf::Vector2i screen_size(SCREEN_WIDTH, SCREEN_HEIGHT);

  // SETUP
  sf::RenderWindow window;
  setup_window(window, screen_size);
  Board board(BOARD_ROWS, BOARD_COLS, TILE_SIZE_PX, screen_size);

  sf::VertexArray turret = get_turret();

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

    for (auto &tile : board.m_tiles) {
      if (point_in_shape(tile.m_shape, sf::Vector2f(mouse_pos))) {
        tile.m_shape.setFillColor(sf::Color(100, 100, 100));
      } else {
        tile.m_shape.setFillColor(sf::Color(0, 0, 0));
      }
    }

    // DRAW
    window.clear();
    board.draw(window);
    window.draw(turret);
    window.display();
  }
}
