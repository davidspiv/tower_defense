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

#endif
