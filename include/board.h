#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum Tile_Role { EMPTY, TURRET, TOWER, SPAWN };

struct Board {
  const size_t rows;
  const size_t cols;
  const float tile_size;

  Board(unsigned w_width, unsigned w_height, float tile_size)
      : rows(64), cols(64), tile_size(tile_size) {}
};

struct Tile {
  sf::Vector2f origin;
  const float size;
  sf::Vector2f center;
  Tile_Role role;
  sf::ConvexShape shape;

  Tile(const sf::Vector2f origin, const Board& board, Tile_Role role)
      : origin(origin),
        size(board.tile_size),
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

    const float isometric_board_half_height =
        (board.cols + board.rows - 1) * (size / 8.f);

    // ~1.7 works, idk why! Golden ratio? Screen aspect ratio?
    const float y_offset = isometric_board_half_height * 1.7;

    shape.setPosition(isometric_origin - sf::Vector2f(0.f, y_offset));
    shape.setFillColor(sf::Color(0, 0, 0));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(250, 150, 100));
  }
};

std::vector<Tile> get_tiles(const Board& board, const float screen_width,
                            const float screen_height) {
  std::vector<Tile> tiles;
  tiles.reserve(board.cols * board.rows);

  const double x_offset = screen_width / 2.f;

  for (size_t row = 0; row < board.rows; ++row) {
    for (size_t col = 0; col < board.cols; ++col) {
      float x = col * board.tile_size + x_offset;
      float y = row * board.tile_size;
      Tile tile(sf::Vector2f(x, y), board, EMPTY);
      tiles.emplace_back(tile);
    }
  }

  return tiles;
}

#endif
