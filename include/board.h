#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>


struct Tile {
  sf::Vector2f m_origin;
  sf::Vector2f m_screen_pos;
  sf::ConvexShape m_top_face;
  sf::ConvexShape m_right_face;
  sf::ConvexShape m_left_face;

  Tile(const sf::Vector2f origin, const unsigned size);

  bool contains(const sf::Vector2f &screen_point, const float tile_size);
};


struct Board {
  const sf::Vector2u m_grid_dim;
  const unsigned m_tile_size;
  const sf::Vector2i m_screen_dim;
  std::vector<Tile> m_tiles;

  Board(sf::Vector2u grid_dim, unsigned tile_size, sf::Vector2i screen_dim);

  sf::Vector2f to_screen_centered_pos(const sf::Vector2f &origin) const;

  // Converts a 2D grid coordinate to a screen-centered isometric position
  sf::Vector2f iso_to_screen(const sf::Vector2f &origin) const;

  // Fills the board with tiles and positions them in isometric layout
  void populate_tiles();

  // Draws all the tiles onto the screen
  void draw(sf::RenderWindow &window);
};


static sf::ConvexShape create_top_face(float size) {
  sf::ConvexShape shape(4);
  float half = size / 2.f;
  shape.setPoint(0, {0.f, -half});
  shape.setPoint(1, {size, 0.f});
  shape.setPoint(2, {0.f, half});
  shape.setPoint(3, {-size, 0.f});

  shape.setFillColor(sf::Color(52, 95, 60));
  shape.setOutlineThickness(-2.f);
  shape.setOutlineColor(sf::Color(93, 171, 108));

  return shape;
}


static sf::ConvexShape create_right_face(float size) {
  sf::ConvexShape shape(4);
  float w = size;
  float h = size / 2.f;
  float z = size;

  shape.setPoint(0, {w, 0.f});
  shape.setPoint(1, {0.f, h});
  shape.setPoint(2, {0.f, h + z});
  shape.setPoint(3, {w, z});

  shape.setFillColor(sf::Color(78, 46, 25));
  shape.setOutlineThickness(-2.f);
  shape.setOutlineColor(sf::Color(154, 91, 49));

  return shape;
}


static sf::ConvexShape create_left_face(float size) {
  sf::ConvexShape shape(4);
  float w = size;
  float h = size / 2.f;
  float z = size;

  shape.setPoint(0, {-w, 0.f});
  shape.setPoint(1, {0.f, h});
  shape.setPoint(2, {0.f, h + z});
  shape.setPoint(3, {-w, z});

  shape.setFillColor(sf::Color(139, 99, 64));
  shape.setOutlineThickness(-2.f);
  shape.setOutlineColor(sf::Color(215, 153, 99));

  return shape;
}


Tile::Tile(const sf::Vector2f origin, const unsigned size)
    : m_origin(origin), m_top_face(sf::ConvexShape(4)),
      m_right_face(sf::ConvexShape(4)), m_left_face(sf::ConvexShape(4)) {

  float size_f = static_cast<float>(size);

  m_top_face = create_top_face(size_f);
  m_right_face = create_right_face(size_f);
  m_left_face = create_left_face(size_f);
}


bool Tile::contains(const sf::Vector2f &screen_point, const float tile_size) {
  const sf::Vector2f screen_pos =
      screen_point - m_screen_pos + sf::Vector2f(0.f, tile_size / 2.f);

  float x = (2.f * screen_pos.y + screen_pos.x) / 2.f;
  float y = (2.f * screen_pos.y - screen_pos.x) / 2.f;

  return (x >= 0.f && x <= tile_size && y >= 0.f && y <= tile_size);
}


Board::Board(sf::Vector2u grid_dim, const unsigned tile_size,
             sf::Vector2i screen_dim)
    : m_grid_dim(grid_dim), m_tile_size(tile_size),
      m_screen_dim(sf::Vector2f(screen_dim)) {
  populate_tiles();
}


sf::Vector2f Board::to_screen_centered_pos(const sf::Vector2f &origin) const {
  float iso_board_height =
      (m_grid_dim.x + m_grid_dim.y - 1) * (m_tile_size / 4.f);
  float x_offset = m_screen_dim.x / 2.f;
  float y_offset = (m_screen_dim.y / 2.f - iso_board_height);

  return origin + sf::Vector2f(x_offset, y_offset);
}


sf::Vector2f Board::iso_to_screen(const sf::Vector2f &iso_pos) const {
  return sf::Vector2f(iso_pos.x - iso_pos.y, (iso_pos.x + iso_pos.y) / 2.f);
}


void Board::populate_tiles() {
  m_tiles.reserve(m_grid_dim.x * m_grid_dim.y);

  for (unsigned row = 0; row < m_grid_dim.x; ++row) {
    for (unsigned col = 0; col < m_grid_dim.y; ++col) {
      float x = static_cast<float>(col * m_tile_size);
      float y = static_cast<float>(row * m_tile_size);
      Tile tile(sf::Vector2f(x, y), m_tile_size);

      const sf::Vector2f tile_pos = iso_to_screen(tile.m_origin);
      tile.m_screen_pos = to_screen_centered_pos(tile_pos);
      tile.m_top_face.setPosition(tile.m_screen_pos);
      tile.m_right_face.setPosition(tile.m_screen_pos);
      tile.m_left_face.setPosition(tile.m_screen_pos);

      m_tiles.emplace_back(std::move(tile));
    }
  }
}


void Board::draw(sf::RenderWindow &window) {
  for (auto &tile : m_tiles) {
    window.draw(tile.m_top_face);
    window.draw(tile.m_right_face);
    window.draw(tile.m_left_face);
  }
}


#endif
