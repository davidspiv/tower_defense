#ifndef UTIL_H
#define UTIL_H

#include <SFML/Graphics.hpp>
#include <cmath>


class MouseThrottler {
public:
  MouseThrottler(sf::Time cooldown)
      : cooldown(cooldown), lastClick(sf::Time::Zero) {}

  bool canClick(sf::Clock &clock) {
    sf::Time now = clock.getElapsedTime();
    if (now - lastClick >= cooldown) {
      lastClick = now;
      return true;
    }
    return false;
  }

private:
  sf::Time cooldown;
  sf::Time lastClick;
};


float angle_to(const sf::Vector2f &from, const sf::Vector2f &to) {
  sf::Vector2f dir = to - from;
  return std::atan2(dir.y, dir.x);
}


float shortest_angle_delta(float from, float to) {
  return std::fmod(to - from + 3.f * M_PI, 2.f * M_PI) - M_PI;
}


auto calc_dist(const sf::Vector2f coord_a, const sf::Vector2f coord_b) {
  sf::Vector2f mag = coord_b - coord_a;
  return std::sqrt(mag.x * mag.x + mag.y * mag.y);
}


// set window dimensions and center in desktop environment
void setup_window(sf::RenderWindow &window, sf::Vector2i screen_size) {
  const unsigned width = static_cast<unsigned>(screen_size.x);
  const unsigned height = static_cast<unsigned>(screen_size.y);

  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  window.create(sf::VideoMode(width, height), "Iso Demo", sf::Style::Default,
                settings);

  const auto desktop = sf::VideoMode::getDesktopMode();
  window.setPosition({static_cast<int>(desktop.width / 2 - width / 2),
                      static_cast<int>(desktop.height / 2 - height / 2)});


  std::cout << "Anti-Aliasing: "
            << (window.getSettings().antialiasingLevel ? "ON" : "OFF") << "\n";
}


sf::CircleShape build_circle() {
  sf::CircleShape circle(12.f);
  circle.setFillColor(sf::Color(75, 68, 58));
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


sf::VertexArray
build_primitive_rounded_rect(const sf::Vector2f pos, const sf::Vector2f size,
                             const sf::Color color, const float radius = 20.f,
                             const std::size_t cornerResolution = 8) {
  sf::VertexArray vertices(sf::TriangleFan);

  sf::Vector2f turret_center = pos + size * 0.5f;
  vertices.append(sf::Vertex(turret_center, color)); // Turret_center of fan

  std::vector<sf::Vector2f> arcPoints;

  // Generate arc points for each corner
  auto arc = [&](sf::Vector2f cornerTurret_center, float startAngleRad) {
    for (std::size_t i = 0; i <= cornerResolution; ++i) {
      const float angle =
          startAngleRad + M_PI / 2.f * static_cast<float>(i) / cornerResolution;
      const float x = cornerTurret_center.x + std::cos(angle) * radius;
      const float y = cornerTurret_center.y + std::sin(angle) * radius;
      arcPoints.emplace_back(x, y);
    }
  };

  // Generate corner arcs clockwise
  arc(pos + sf::Vector2f(radius, radius), M_PI); // Top-left
  arc(pos + sf::Vector2f(size.x - radius, radius),
      1.5f * M_PI); // Top-right
  arc(pos + sf::Vector2f(size.x - radius, size.y - radius),
      0.f);                                                      // Bottom-right
  arc(pos + sf::Vector2f(radius, size.y - radius), 0.5f * M_PI); // Bottom-left

  // Append arc points to vertex array
  for (const auto &pt : arcPoints) {
    vertices.append(sf::Vertex(pt, color));
  }
  vertices.append(vertices[2]); // close off turret

  return vertices;
}


#endif
