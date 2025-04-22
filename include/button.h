#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <cmath>

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

struct Button {
  sf::Vector2f size;
  sf::Vector2f pos;
  sf::VertexArray shape;
  sf::Color color_norm;
  sf::Color color_pressed;

  Button()
      : size(100.f, 100.f), pos(100.f, 100.f),
        shape(build_primitive_rounded_rect(pos, size, sf::Color::White)),
        color_norm(sf::Color(100, 100, 100)), color_pressed(sf::Color::White) {}
};


#endif
