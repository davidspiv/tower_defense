#pragma once

#include "util.h"

#include <SFML/Graphics.hpp>

enum Tile_Role { EMPTY, TURRET, TOWER };

struct Tile {
    sf::Vector2f m_origin;
    sf::Vector2f m_screen_pos;
    sf::ConvexShape m_top_face;
    sf::ConvexShape m_right_face;
    sf::ConvexShape m_left_face;
    Tile_Role m_role;

    sf::Color color_default;
    sf::Color color_turret_placement_approved;

    Tile(sf::Vector2f const origin, unsigned const size);

    bool contains(sf::Vector2f const& screen_point, float const tile_size);
};

inline static sf::ConvexShape create_top_face(float size)
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

inline static sf::ConvexShape create_right_face(float size)
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

inline static sf::ConvexShape create_left_face(float size)
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

inline Tile::Tile(sf::Vector2f const origin, unsigned const size)
    : m_origin(origin)
    , m_top_face(sf::ConvexShape(4))
    , m_right_face(sf::ConvexShape(4))
    , m_left_face(sf::ConvexShape(4))
    , m_role(EMPTY)
    , color_default(sf::Color(52, 95, 60))
    , color_turret_placement_approved(sf::Color(93, 171, 108))
{

    float size_f = static_cast<float>(size);

    m_top_face = create_top_face(size_f);
    m_right_face = create_right_face(size_f);
    m_left_face = create_left_face(size_f);
}

inline bool Tile::contains(sf::Vector2f const& screen_point, float const tile_size)
{
    sf::Vector2f const screen_pos
        = screen_point - m_screen_pos + sf::Vector2f(0.f, tile_size / 2.f);

    float x = (2.f * screen_pos.y + screen_pos.x) / 2.f;
    float y = (2.f * screen_pos.y - screen_pos.x) / 2.f;

    return (x >= 0.f && x <= tile_size && y >= 0.f && y <= tile_size);
}
