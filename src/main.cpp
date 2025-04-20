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

struct Turret {
  sf::VertexArray path;
  sf::Vector2f center;
  float curr_angle;
  float rotation_speed;

  int fire_timer;

  Turret(const sf::Vector2f tile_center)
      : path(sf::TriangleFan), curr_angle(90.f), rotation_speed(0.5f),
        fire_timer(500) {

    const static std::vector<sf::Vector2f> points = {
        {102.5f, 0.0f}, // Top center
        {122.5f, 2.0f},   {142.0f, 8.0f},   {160.0f, 20.0f},  {175.0f, 35.0f},
        {185.0f, 55.0f},  {192.0f, 75.0f},  {197.0f, 100.0f}, {200.0f, 125.0f},
        {200.5f, 149.5f}, // Right mid

        {198.0f, 165.0f}, {192.0f, 180.0f}, {182.0f, 190.0f}, {165.0f, 195.0f},
        {140.0f, 198.0f}, {122.0f, 199.5f}, {102.5f, 200.0f}, // Bottom center

        {83.0f, 199.5f},  {65.0f, 198.0f},  {40.0f, 195.0f},  {23.0f, 190.0f},
        {13.0f, 180.0f},  {7.0f, 165.0f},   {4.0f, 149.5f}, // Left mid

        {4.0f, 125.0f},   {7.0f, 100.0f},   {13.0f, 75.0f},   {23.0f, 55.0f},
        {38.0f, 35.0f},   {55.0f, 20.0f},   {73.0f, 8.0f},    {92.5f, 2.0f},
        {102.5f, 0.0f} // Closing at top center
    };

    for (auto point : points) {
      path.append(sf::Vertex(point, sf::Color(200, 200, 200)));
    }

    // Closing loop
    path.append(sf::Vertex(path[1]));


    for (const auto &pt : points) {
      path.append(sf::Vertex(pt));
    }

    // 2. Move the shape so it's centered around (0,0)
    for (std::size_t i = 0; i < path.getVertexCount(); ++i) {
      path[i].position -= {points[0].x, 175.f}; // shape center
    }

    // 3. Then we'll place it on the desired tile center
    sf::Transform transform;
    transform.translate(tile_center);
    transform.scale(0.15f, 0.15f);

    center = tile_center;

    for (std::size_t i = 0; i < path.getVertexCount(); ++i) {
      path[i].position = transform.transformPoint(path[i].position);
    }
  }
};

int main() {
  sf::Vector2i screen_size(SCREEN_WIDTH, SCREEN_HEIGHT);

  // SETUP
  sf::RenderWindow window;
  setup_window(window, screen_size);
  Board board(BOARD_ROWS, BOARD_COLS, TILE_SIZE_PX, screen_size);

  std::vector<Turret> turrets;

  for (auto &tile : board.m_tiles) {
    turrets.emplace_back(Turret(tile.m_shape.getPosition()));
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


    for (auto &turret : turrets) {
      window.draw(turret.path);
    }


    window.display();
  }
}
