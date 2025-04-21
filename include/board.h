#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>


struct Tile {
  sf::Vector2f m_origin;
  sf::Vector2f m_screen_pos;
  sf::ConvexShape m_shape;

  Tile(const sf::Vector2f origin, const unsigned size);
};


struct Board {
  const sf::Vector2u m_grid_dim;
  const unsigned m_tile_size;
  const sf::Vector2i m_screen_dim;
  std::vector<Tile> m_tiles;

  Board(sf::Vector2u grid_dim, unsigned tile_size, sf::Vector2i screen_dim);

  sf::Vector2f to_screen_centered_pos(const sf::Vector2f &origin) const;

  // Converts a 2D grid coordinate to a screen-centered isometric position
  sf::Vector2f to_iso_pos(const sf::Vector2f &origin) const;

  // Fills the board with tiles and positions them in isometric layout
  void populate_tiles();

  // Draws all the tiles onto the screen
  void draw(sf::RenderWindow &window);
};


Tile::Tile(const sf::Vector2f origin, const unsigned size)
    : m_origin(origin), m_shape(sf::ConvexShape(4)) {

  const float size_f = static_cast<float>(size);

  m_shape.setPoint(0, sf::Vector2f(0.f, -size_f / 2.f));
  m_shape.setPoint(1, sf::Vector2f(size_f, 0.f));
  m_shape.setPoint(2, sf::Vector2f(0.f, size_f / 2.f));
  m_shape.setPoint(3, sf::Vector2f(-size_f, 0.f));

  m_shape.setFillColor(sf::Color(0, 0, 0));
  m_shape.setOutlineThickness(2.f);
  m_shape.setOutlineColor(sf::Color(250, 150, 100));
}


Board::Board(sf::Vector2u grid_dim, const unsigned tile_size,
             sf::Vector2i screen_dim)
    : m_grid_dim(grid_dim), m_tile_size(tile_size),
      m_screen_dim(sf::Vector2f(screen_dim)) {
  populate_tiles();
};

sf::Vector2f Board::to_screen_centered_pos(const sf::Vector2f &origin) const {
  float iso_board_height =
      (m_grid_dim.x + m_grid_dim.y - 1) * (m_tile_size / 4.f);
  float x_offset = m_screen_dim.x / 2.f;
  float y_offset = (m_screen_dim.y / 2.f - iso_board_height);

  return origin + sf::Vector2f(x_offset, y_offset);
}


sf::Vector2f Board::to_iso_pos(const sf::Vector2f &origin) const {
  return sf::Vector2f(origin.x - origin.y, (origin.x + origin.y) / 2.f);
}


void Board::populate_tiles() {
  m_tiles.reserve(m_grid_dim.x * m_grid_dim.y);

  for (unsigned row = 0; row < m_grid_dim.x; ++row) {
    for (unsigned col = 0; col < m_grid_dim.y; ++col) {
      float x = static_cast<float>(col * m_tile_size);
      float y = static_cast<float>(row * m_tile_size);
      Tile tile(sf::Vector2f(x, y), m_tile_size);

      const sf::Vector2f tile_pos = to_iso_pos(tile.m_origin);
      tile.m_screen_pos = to_screen_centered_pos(tile_pos);
      tile.m_shape.setPosition(tile.m_screen_pos);

      m_tiles.emplace_back(std::move(tile));
    }
  }
}


void Board::draw(sf::RenderWindow &window) {
  for (auto &tile : m_tiles) {
    window.draw(tile.m_shape);
  }
}


#endif
