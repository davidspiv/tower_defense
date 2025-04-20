#include <SFML/Graphics.hpp>
#include <vector>

#include "../include/board.h"
#include "../include/util.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BOARD_ROWS 5
#define BOARD_COLS 5
#define TILE_SIZE_PX 100 // height gets halved in iso-space

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

  Turret(const sf::Vector2f tile_center, const float tile_size)
      : path(sf::TriangleFan), curr_angle(90.f), rotation_speed(0.5f),
        fire_timer(500) {

    const static std::vector<sf::Vector2f> points = {
        {1, 0.74771},           {0.990843, 0.800013},   {0.964488, 0.846115},
        {0.926444, 0.883253},   {0.881982, 0.912455},   {0.834055, 0.935549},
        {0.78415, 0.953976},    {0.733035, 0.968714},   {0.681142, 0.980402},
        {0.628722, 0.989433},   {0.575939, 0.995989},   {0.522904, 1},
        {0.469737, 0.999581},   {0.416667, 0.996043},   {0.363817, 0.990046},
        {0.311315, 0.981519},   {0.259344, 0.970184},   {0.208199, 0.955561},
        {0.158379, 0.93691},    {0.110794, 0.913128},   {0.0671886, 0.882674},
        {0.0308953, 0.843836},  {0.00714522, 0.796329}, {0, 0.743659},
        {0.00228147, 0.690335}, {0.00786696, 0.637252}, {0.0162233, 0.584537},
        {0.0270923, 0.532284},  {0.0403624, 0.480592},  {0.0560179, 0.429578},
        {0.0741149, 0.379384},  {0.0947685, 0.330195},  {0.118145, 0.282252},
        {0.144461, 0.235871},   {0.17397, 0.19147},     {0.206948, 0.149605},
        {0.243654, 0.110999},   {0.284256, 0.0765581},  {0.328729, 0.0473406},
        {0.376725, 0.024432},   {0.427509, 0.00871545}, {0.480036, 0.000611327},
        {0.533182, 0},          {0.585854, 0.00707261}, {0.636818, 0.0221631},
        {0.684873, 0.0449321},  {0.729146, 0.0744437},  {0.769237, 0.109478},
        {0.80514, 0.148834},    {0.837087, 0.191494},   {0.865408, 0.236666},
        {0.890446, 0.283753},   {0.912511, 0.332318},   {0.931866, 0.382035},
        {0.948715, 0.432663},   {0.963208, 0.484023},   {0.975431, 0.535974},
        {0.9854, 0.588407},     {0.993032, 0.641234},   {0.998085, 0.694371}};

    for (auto point : points) {
      path.append(sf::Vertex(point, sf::Color(200, 200, 200)));
    }

    // Closing loop
    path.append(sf::Vertex(path[1]));


    for (const auto &pt : points) {
      path.append(sf::Vertex(pt));
    }

    // // 2. Move the shape so it's centered around (0,0)
    for (std::size_t i = 0; i < path.getVertexCount(); ++i) {
      path[i].position -= {.5f, .8f};
    }

    // 3. Then we'll place it on the desired tile center
    sf::Transform transform;
    transform.translate(tile_center);
    transform.scale(tile_size / 1.2f, tile_size / 1.2f);

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
