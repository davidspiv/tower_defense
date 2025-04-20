#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>

struct Tile {
  sf::Vector2f m_origin;
  float m_size;
  sf::ConvexShape m_shape;

  Tile(sf::Vector2f origin, float size);
};

class Board {
private:
  const size_t m_rows;
  const size_t m_cols;
  const float m_tile_size; // in grid-space
  const sf::Vector2f m_screen_size;

public:
  std::vector<Tile> m_tiles;

  Board(unsigned rows, unsigned cols, float tile_size,
        sf::Vector2i screen_size);

  // Converts a 2D grid coordinate to a screen-centered isometric position
  sf::Vector2f to_screen_isometric_position(const sf::Vector2f &origin) const;

  // Fills the board with tiles and positions them in isometric layout
  void populate_tiles();

  // Draws all the tiles onto the screen
  void draw(sf::RenderWindow &window);
};

Tile::Tile(sf::Vector2f origin, float size)
    : m_origin(origin), m_size(size), m_shape(sf::ConvexShape(4)) {
  m_shape.setPoint(0, sf::Vector2f(0.f, -size / 2.f));
  m_shape.setPoint(1, sf::Vector2f(size, 0.f));
  m_shape.setPoint(2, sf::Vector2f(0.f, size / 2.f));
  m_shape.setPoint(3, sf::Vector2f(-size, 0.f));

  m_shape.setFillColor(sf::Color(0, 0, 0));
  m_shape.setOutlineThickness(2.f);
  m_shape.setOutlineColor(sf::Color(250, 150, 100));
}

Board::Board(unsigned rows, unsigned cols, float tile_size,
             sf::Vector2i screen_size)
    : m_rows(rows), m_cols(cols), m_tile_size(tile_size),
      m_screen_size(sf::Vector2f(screen_size)) {
  populate_tiles();
};

sf::Vector2f
Board::to_screen_isometric_position(const sf::Vector2f &origin) const {
  auto to_screen_center = [*this](const sf::Vector2f &origin) {
    float iso_board_height = (m_cols + m_rows - 1) * (m_tile_size / 4.f);
    float x_offset = m_screen_size.x / 2.f;
    float y_offset = m_screen_size.y / 2.f - iso_board_height;

    return origin + sf::Vector2f(x_offset, y_offset);
  };

  const sf::Vector2f iso_pos(origin.x - origin.y, (origin.x + origin.y) / 2.f);

  return to_screen_center(iso_pos);
}

void Board::populate_tiles() {
  m_tiles.reserve(m_cols * m_rows);

  for (size_t row = 0; row < m_rows; ++row) {
    for (size_t col = 0; col < m_cols; ++col) {
      float x = col * m_tile_size;
      float y = row * m_tile_size;
      Tile tile(sf::Vector2f(x, y), m_tile_size);

      const sf::Vector2f tile_pos = to_screen_isometric_position(tile.m_origin);
      tile.m_shape.setPosition(tile_pos);

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
