#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum Tile_Role { EMPTY, TURRET, TOWER, SPAWN };

struct Tile {
  sf::Vector2f origin;
  const float size;
  Tile_Role role;
  sf::ConvexShape shape;

  Tile(const sf::Vector2f origin, const float size, Tile_Role role)
      : origin(origin), size(size), role(role), shape(sf::ConvexShape(4.f)) {
    shape.setPoint(0, sf::Vector2f(0.f, -size / 2.f));
    shape.setPoint(1, sf::Vector2f(size, 0.f));
    shape.setPoint(2, sf::Vector2f(0.f, size / 2.f));
    shape.setPoint(3, sf::Vector2f(-size, 0.f));

    shape.setFillColor(sf::Color(0, 0, 0));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(250, 150, 100));
  }
};

struct Board {
  const size_t rows;
  const size_t cols;
  const float tile_size;
  std::vector<Tile> tiles;

  Board(unsigned rows, unsigned cols, float tile_size, float screen_width,
        float screen_height)
      : rows(rows), cols(cols), tile_size(tile_size) {
    populate_tiles(screen_width, screen_height);
  }

  sf::Vector2f calc_centered_isometric_origin(Tile& tile,
                                              const float screen_width,
                                              const float screen_height) {
    // Isometric transform
    sf::Vector2f isometric_origin{tile.origin.x - tile.origin.y,
                                  (tile.origin.x + tile.origin.y) / 2.f};

    const float isometric_board_height = (cols + rows - 1) * (tile_size / 4.f);

    const float x_offset = screen_width / 2.f;
    const float y_offset = (screen_height / 2.f) - isometric_board_height;

    return isometric_origin + sf::Vector2f(x_offset, y_offset);
  }

  void populate_tiles(const float screen_width, const float screen_height) {
    tiles.reserve(cols * rows);

    for (size_t row = 0; row < rows; ++row) {
      for (size_t col = 0; col < cols; ++col) {
        float x = col * tile_size;
        float y = row * tile_size;
        Tile tile(sf::Vector2f(x, y), tile_size, EMPTY);
        const sf::Vector2f tile_pos =
            calc_centered_isometric_origin(tile, screen_width, screen_height);
        tile.shape.setPosition(tile_pos);
        tiles.emplace_back(tile);
      }
    }
  }
};

#endif
