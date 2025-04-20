#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum Tile_Role { EMPTY, TURRET, TOWER, SPAWN };

struct Tile {
  sf::Vector2f origin;
  const float size;
  sf::Vector2f center;
  Tile_Role role;
  sf::ConvexShape shape;

  Tile(const sf::Vector2f origin, const float size, Tile_Role role)
      : origin(origin),
        size(size),
        center(origin.x + size / 2.f, origin.y + size / 2.f),
        role(role),
        shape(sf::ConvexShape(4.f)) {
    shape.setPoint(0, sf::Vector2f(0.f, -size / 2.f));
    shape.setPoint(1, sf::Vector2f(size, 0.f));
    shape.setPoint(2, sf::Vector2f(0.f, size / 2.f));
    shape.setPoint(3, sf::Vector2f(-size, 0.f));

    // Isometric transform
    sf::Vector2f isometric_origin{origin.x - origin.y,
                                  (origin.x + origin.y) / 2.f};

    shape.setPosition(isometric_origin + sf::Vector2f(size / 2.f, size / 2.f) +
                      sf::Vector2f(4 * size, -1.5 * size));
    shape.setFillColor(sf::Color(0, 0, 0));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(250, 150, 100));
  }
};

struct Board {
  const size_t rows;
  const size_t cols;
  const float tile_size;

  Board(unsigned w_width, unsigned w_height, float tile_size)
      : rows(3), cols(3), tile_size(tile_size) {}
};

std::vector<Tile> get_tiles(const Board& board, const float screen_width,
                            const float screen_height) {
  std::vector<Tile> tiles;
  tiles.reserve(board.cols * board.rows);

  const float x_offset =
      screen_width / 2.f - (board.tile_size * board.cols / 2.f);
  const float y_offset =
      screen_height / 2.f - (board.tile_size * board.rows / 2.f);

  for (size_t row = 0; row < board.rows; ++row) {
    for (size_t col = 0; col < board.cols; ++col) {
      float x = col * board.tile_size + x_offset;
      float y = row * board.tile_size + y_offset;
      Tile tile(sf::Vector2f(x, y), board.tile_size, EMPTY);
      tiles.emplace_back(tile);
    }
  }

  return tiles;
}

#endif
