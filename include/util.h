#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>

auto calc_dist(const sf::Vector2f coord_a, const sf::Vector2f coord_b) {
  sf::Vector2f mag = coord_b - coord_a;
  return std::sqrt(mag.x * mag.x + mag.y * mag.y);
}

float to_degrees(const float radians) { return radians * (180.0 / M_PI); }

void setup_window(sf::RenderWindow& window, const unsigned w_width,
                  const unsigned w_height) {
  window.create(sf::VideoMode(w_width, w_height), "TD_test");

  auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition({static_cast<int>(desktop.width / 2 - w_width / 2),
                      static_cast<int>(desktop.height / 2 - w_height / 2)});
}

sf::ConvexShape make_horizontal_hexagon(sf::Vector2f position,
                                        float tile_size) {
  float w = tile_size;
  float h = tile_size / 2.f;

  sf::ConvexShape hex;
  hex.setPointCount(6);

  hex.setPoint(0, sf::Vector2f(0.f, -h));             // top
  hex.setPoint(1, sf::Vector2f(w / 2.f, -h / 2.f));   // top-right
  hex.setPoint(2, sf::Vector2f(w / 2.f, h / 2.f));    // bottom-right
  hex.setPoint(3, sf::Vector2f(0.f, h));              // bottom
  hex.setPoint(4, sf::Vector2f(-w / 2.f, h / 2.f));   // bottom-left
  hex.setPoint(5, sf::Vector2f(-w / 2.f, -h / 2.f));  // top-left

  hex.setPosition(position);

  return hex;
}

#endif
