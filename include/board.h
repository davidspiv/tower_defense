#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum Tile_Role { EMPTY, TURRET, TOWER, SPAWN };

struct Tile {
  const sf::Vector2f center;
  const float size;
  Tile_Role role;
  sf::Vector2f origin;
  sf::RectangleShape shape;

  Tile(const sf::Vector2f center, const float size, Tile_Role role)
      : center(center),
        size(size),
        role(role),
        origin(center - sf::Vector2f(size / 2.f, size / 2.f)),
        shape(sf::RectangleShape(sf::Vector2f(size, size))) {
    shape.setPosition(origin);
    shape.setFillColor(sf::Color(0, 0, 0));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(250, 150, 100));
  }
};

struct Board {
  const float margin;
  const size_t cols;
  const size_t rows;
  const float tile_size;

  Board(unsigned w_width, unsigned w_height, float tile_size)
      : margin(2.f * tile_size),
        cols(static_cast<size_t>((w_width - 4.f * tile_size) / tile_size)),
        rows(static_cast<size_t>((w_height - 4.f * tile_size) / tile_size)),
        tile_size(tile_size) {}
};

std::vector<Tile> get_tiles(const Board& board) {
  std::vector<Tile> tiles;
  tiles.reserve(board.cols * board.rows);

  for (size_t row = 0; row < board.rows; ++row) {
    for (size_t col = 0; col < board.cols; ++col) {
      float x = board.margin + col * board.tile_size + board.tile_size * 0.5f;
      float y = board.margin + row * board.tile_size + board.tile_size * 0.5f;
      Tile tile = {sf::Vector2f(x, y), board.tile_size, EMPTY};
      tiles.emplace_back(tile);
    }
  }

  return tiles;
}

sf::VertexArray build_grid(const Board& board) {
  sf::VertexArray lines(sf::Lines);

  const float right = board.margin + board.cols * board.tile_size;
  const float bottom = board.margin + board.rows * board.tile_size;

  // Vertical lines
  for (size_t col = 0; col <= board.cols; ++col) {
    float x = board.margin + col * board.tile_size;
    lines.append(sf::Vertex(sf::Vector2f(x, board.margin), sf::Color::White));
    lines.append(sf::Vertex(sf::Vector2f(x, bottom), sf::Color::White));
  }

  // Horizontal lines
  for (size_t row = 0; row <= board.rows; ++row) {
    float y = board.margin + row * board.tile_size;
    lines.append(sf::Vertex(sf::Vector2f(board.margin, y), sf::Color::White));
    lines.append(sf::Vertex(sf::Vector2f(right, y), sf::Color::White));
  }

  return lines;
}

#endif
