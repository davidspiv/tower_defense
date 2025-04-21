#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>


// set window dimensions and center in desktop environment
void setup_window(sf::RenderWindow &window, sf::Vector2i screen_size) {
  const unsigned width = static_cast<unsigned>(screen_size.x);
  const unsigned height = static_cast<unsigned>(screen_size.y);

  window.create(sf::VideoMode(width, height), "Iso Demo");

  const auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition({static_cast<int>(desktop.width / 2 - width / 2),
                      static_cast<int>(desktop.height / 2 - height / 2)});
}


sf::CircleShape build_circle() {
  sf::CircleShape circle(12.f);
  circle.setFillColor(sf::Color(100, 100, 100));
  circle.setOrigin(10.f, 10.f);

  return circle;
}


// LLM code, probably should just store grid-space and iso-space coords in tile
bool point_in_iso_tile(const sf::Vector2f &screen_point, const Tile &tile,
                       const float tile_size) {
  sf::Vector2f screen_pos =
      screen_point - tile.m_screen_pos + sf::Vector2f(0.f, tile_size / 2.f);

  float x = (2.f * screen_pos.y + screen_pos.x) / 2.f;
  float y = (2.f * screen_pos.y - screen_pos.x) / 2.f;

  return (x >= 0.f && x <= tile_size && y >= 0.f && y <= tile_size);
}

#endif
