#ifndef TILE_H
#define TILE_H


#include "util.h"

#include <SFML/Graphics.hpp>

enum Tile_Role { EMPTY, TURRET, TOWER };

struct Tile {
  sf::Vector2f m_origin;
  sf::Vector2f m_screen_pos;
  sf::ConvexShape m_top_face;
  sf::ConvexShape m_right_face;
  sf::ConvexShape m_left_face;
  Tile_Role m_role;

  sf::Color color_default;
  sf::Color color_turret_placement_approved;

  Tile(const sf::Vector2f origin, const unsigned size);

  bool contains(const sf::Vector2f &screen_point, const float tile_size);
};


Tile::Tile(const sf::Vector2f origin, const unsigned size)
    : m_origin(origin), m_top_face(sf::ConvexShape(4)),
      m_right_face(sf::ConvexShape(4)), m_left_face(sf::ConvexShape(4)),
      m_role(EMPTY), color_default(sf::Color(52, 95, 60)),
      color_turret_placement_approved(sf::Color(93, 171, 108)) {

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

#endif
