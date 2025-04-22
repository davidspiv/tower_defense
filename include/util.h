#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>
#include <cmath>


float to_degrees(const float radians) { return radians * (180.0 / M_PI); }


float angle_to(const sf::Vector2f &from, const sf::Vector2f &to) {
  sf::Vector2f dir = to - from;
  return to_degrees(std::atan2(dir.y, dir.x)) + 90.f;
}


float shortest_angle_delta(float from, float to) {
  return std::fmod(to - from + 540.f, 360.f) - 180.f;
}


auto calc_dist(const sf::Vector2f coord_a, const sf::Vector2f coord_b) {
  sf::Vector2f mag = coord_b - coord_a;
  return std::sqrt(mag.x * mag.x + mag.y * mag.y);
}


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
  circle.setOrigin(circle.getRadius(), circle.getRadius());

  return circle;
}


sf::CircleShape build_circle(const sf::Vector2f pos, sf::Color color,
                             float radius) {
  sf::CircleShape shape(radius);
  shape.setPosition(pos.x - radius, pos.y - radius);
  shape.setFillColor(color);
  shape.setOrigin(shape.getRadius(), shape.getRadius());
  return shape;
}


#endif
