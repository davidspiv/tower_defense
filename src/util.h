#pragma once

#define _USE_MATH_DEFINES // for visual studio

#include "config.h"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

inline float angle_to(sf::Vector2f const& from, sf::Vector2f const& to)
{
    sf::Vector2f dir = to - from;
    return std::atan2(dir.y, dir.x);
}

inline float shortest_angle_delta(float from, float to)
{
    return std::fmod(to - from + 3.f * M_PI, 2.f * M_PI) - M_PI;
}

inline auto calc_dist(sf::Vector2f const coord_a, sf::Vector2f const coord_b)
{
    sf::Vector2f mag = coord_b - coord_a;
    return std::sqrt(mag.x * mag.x + mag.y * mag.y);
}

sf::CircleShape build_circle(sf::Vector2f const pos, sf::Color color, float radius)
{
    sf::CircleShape shape(radius);
    shape.setPosition(pos.x - radius, pos.y - radius);
    shape.setFillColor(color);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    return shape;
}
