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
      : rows(4), cols(4), tile_size(tile_size) {}
};

struct Tile {
  sf::Vector2f origin;
  const float size;
  Tile_Role role;
  sf::ConvexShape shape;

  Tile(const sf::Vector2f origin, const Board& board, const float screen_width,
       const float screen_height, Tile_Role role)
      : origin(origin),
        size(board.tile_size),
        role(role),
        shape(sf::ConvexShape(4.f)) {
    shape.setPoint(0, sf::Vector2f(0.f, -size / 2.f));
    shape.setPoint(1, sf::Vector2f(size, 0.f));
    shape.setPoint(2, sf::Vector2f(0.f, size / 2.f));
    shape.setPoint(3, sf::Vector2f(-size, 0.f));

    // Isometric transform
    sf::Vector2f isometric_origin{origin.x - origin.y,
                                  (origin.x + origin.y) / 2.f};

    const float x_offset = screen_width / 2.f;

    const float isometric_board_height =
        (board.cols + board.rows - 1) * (size / 8.f);

    const float y_offset = 0;

    shape.setPosition(isometric_origin + sf::Vector2f(x_offset, y_offset));
    shape.setFillColor(sf::Color(0, 0, 0));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(250, 150, 100));
  }
};

std::vector<Tile> get_tiles(const Board& board, const float screen_width,
                            const float screen_height) {
  std::vector<Tile> tiles;
  tiles.reserve(board.cols * board.rows);

  for (size_t row = 0; row < board.rows; ++row) {
    for (size_t col = 0; col < board.cols; ++col) {
      float x = col * board.tile_size;
      float y = row * board.tile_size;
      Tile tile(sf::Vector2f(x, y), board, screen_width, screen_height, EMPTY);
      tiles.emplace_back(tile);
    }
  }

  return tiles;
}

#endif
