#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>
#include <cmath>


// set window dimensions and center in desktop environment
void setup_window(sf::RenderWindow &window, sf::Vector2i screen_size) {
  const unsigned width = static_cast<unsigned>(screen_size.x);
  const unsigned height = static_cast<unsigned>(screen_size.y);

  window.create(sf::VideoMode(width, height), "Iso Demo");

  const auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition({static_cast<int>(desktop.width / 2 - width / 2),
                      static_cast<int>(desktop.height / 2 - height / 2)});
}


auto calc_dist(const sf::Vector2f coord_a, const sf::Vector2f coord_b) {
  sf::Vector2f mag = coord_b - coord_a;
  return std::sqrt(mag.x * mag.x + mag.y * mag.y);
}


sf::CircleShape build_circle() {
  sf::CircleShape circle(12.f);
  circle.setFillColor(sf::Color(100, 100, 100));
  circle.setOrigin(10.f, 10.f);

  return circle;
}


sf::CircleShape build_circle(const sf::Vector2f pos, sf::Color color,
                             float radius) {
  sf::CircleShape shape(radius);
  shape.setPosition(pos.x - radius, pos.y - radius);
  shape.setFillColor(color);
  return shape;
}


#endif
