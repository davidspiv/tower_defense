#pragma once

#include "util.h"

#include <SFML/Graphics.hpp>
#include <cmath>

struct Button {
    sf::Vector2f size;
    sf::Vector2f pos;
    sf::VertexArray shape;
    sf::Color color_norm = sf::Color(189, 181, 155);
    sf::Color color_pressed = sf::Color(112, 107, 92);
    bool turret_selected = false;
    bool was_hovered = false;

    Button(sf::Vector2i const screen_dim);
    bool is_hovered(sf::Vector2i const& mouse_pos) const;
    void set_fill_color(sf::Color const& color);
    void update(sf::Vector2i const& mouse_pos, bool clicked);
};

inline sf::VertexArray build_primitive_rounded_rect(sf::Vector2f const pos, sf::Vector2f const size,
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

inline Button::Button(sf::Vector2i const screen_dim)
    : size(100.f, 100.f)
    , pos(screen_dim.x / 2.f - size.x / 2.f, screen_dim.y - 150.f)
{
    shape = build_primitive_rounded_rect(pos, size, color_norm);
    set_fill_color(color_norm);
}

inline bool Button::is_hovered(sf::Vector2i const& mouse_pos) const
{
    return shape.getBounds().contains(static_cast<sf::Vector2f>(mouse_pos));
}

inline void Button::set_fill_color(sf::Color const& color)
{
    for (std::size_t i = 0; i < shape.getVertexCount(); ++i) {
        shape[i].color = color;
    }
}

inline void Button::update(sf::Vector2i const& mouse_pos, bool clicked)
{
    if (!is_hovered(mouse_pos) && clicked) {
        set_fill_color(color_norm);
        turret_selected = false;

    } else if (turret_selected || (is_hovered(mouse_pos) && clicked)) {

        set_fill_color(color_pressed);
        turret_selected = true;
        was_hovered = true;

    } else if (was_hovered) {
        set_fill_color(color_norm);
    }
}
