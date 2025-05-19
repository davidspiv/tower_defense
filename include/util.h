#ifndef UTIL_H
#define UTIL_H
#define _USE_MATH_DEFINES // for visual studio

#include "config.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

class MouseThrottler {
public:
    MouseThrottler(sf::Time cooldown)
        : cooldown(cooldown)
        , lastClick(sf::Time::Zero)
    {
    }

    bool canClick(sf::Clock& clock)
    {
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

float angle_to(sf::Vector2f const& from, sf::Vector2f const& to)
{
    sf::Vector2f dir = to - from;
    return std::atan2(dir.y, dir.x);
}

float shortest_angle_delta(float from, float to)
{
    return std::fmod(to - from + 3.f * M_PI, 2.f * M_PI) - M_PI;
}

auto calc_dist(sf::Vector2f const coord_a, sf::Vector2f const coord_b)
{
    sf::Vector2f mag = coord_b - coord_a;
    return std::sqrt(mag.x * mag.x + mag.y * mag.y);
}

sf::VertexArray build_primitive_rounded_rect(sf::Vector2f const pos, sf::Vector2f const size,
    sf::Color const color, float const radius = 20.f, std::size_t const cornerResolution = 8)
{
    sf::VertexArray vertices(sf::TriangleFan);

    sf::Vector2f turret_center = pos + size * 0.5f;
    vertices.append(sf::Vertex(turret_center, color)); // Turret_center of fan

    std::vector<sf::Vector2f> arcPoints;

    // Generate arc points for each corner
    auto arc = [&](sf::Vector2f cornerTurret_center, float startAngleRad) {
        for (std::size_t i = 0; i <= cornerResolution; ++i) {
            float const angle
                = startAngleRad + M_PI / 2.f * static_cast<float>(i) / cornerResolution;
            float const x = cornerTurret_center.x + std::cos(angle) * radius;
            float const y = cornerTurret_center.y + std::sin(angle) * radius;
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
    for (auto const& pt : arcPoints) {
        vertices.append(sf::Vertex(pt, color));
    }
    vertices.append(vertices[2]); // close off turret

    return vertices;
}

static sf::ConvexShape create_top_face(float size)
{
    sf::ConvexShape shape(4);
    float half = size / 2.f;
    shape.setPoint(0, { 0.f, -half });
    shape.setPoint(1, { size, 0.f });
    shape.setPoint(2, { 0.f, half });
    shape.setPoint(3, { -size, 0.f });

    shape.setFillColor(sf::Color(52, 95, 60));
    shape.setOutlineThickness(-2.f);
    shape.setOutlineColor(sf::Color(93, 171, 108));

    return shape;
}

static sf::ConvexShape create_right_face(float size)
{
    sf::ConvexShape shape(4);
    float w = size;
    float h = size / 2.f;
    float z = size;

    shape.setPoint(0, { w, 0.f });
    shape.setPoint(1, { 0.f, h });
    shape.setPoint(2, { 0.f, h + z });
    shape.setPoint(3, { w, z });

    shape.setFillColor(sf::Color(78, 46, 25));
    shape.setOutlineThickness(-2.f);
    shape.setOutlineColor(sf::Color(154, 91, 49));

    return shape;
}

static sf::ConvexShape create_left_face(float size)
{
    sf::ConvexShape shape(4);
    float w = size;
    float h = size / 2.f;
    float z = size;

    shape.setPoint(0, { -w, 0.f });
    shape.setPoint(1, { 0.f, h });
    shape.setPoint(2, { 0.f, h + z });
    shape.setPoint(3, { -w, z });

    shape.setFillColor(sf::Color(139, 99, 64));
    shape.setOutlineThickness(-2.f);
    shape.setOutlineColor(sf::Color(215, 153, 99));

    return shape;
}

void setup_window(sf::RenderWindow& window)
{
    unsigned const width = static_cast<unsigned>(SCREEN_WIDTH);
    unsigned const height = static_cast<unsigned>(SCREEN_HEIGHT);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(width, height), "Iso Demo", sf::Style::Default, settings);

    auto const desktop = sf::VideoMode::getDesktopMode();
    window.setPosition({ static_cast<int>(desktop.width / 2 - width / 2),
        static_cast<int>(desktop.height / 2 - height / 2) });

    window.setFramerateLimit(TARGET_FPS);

    std::cout << "Anti-Aliasing: " << (window.getSettings().antialiasingLevel ? "ON" : "OFF")
              << "\n";
}

#endif
