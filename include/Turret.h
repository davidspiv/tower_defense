#ifndef TURRET_H
#define TURRET_H

#include "../include/Bullet.h"
#include "../include/util.h"

#include <SFML/Graphics.hpp>

struct Turret {
    sf::VertexArray base_shape;
    int fire_timer;
    sf::Vector2f center_of_home_tile;

    sf::CircleShape barrel_shape;
    sf::Vector2f barrel_anchor;
    float barrel_rotation_speed;
    float barrel_ellipse_width;
    float barrel_ellipse_height;
    float barrel_angle;

    Turret(sf::Vector2f const tile_center, unsigned const tile_size);
    void update(std::vector<Enemy>& enemies, std::vector<Bullet>& bullets);
    void update();
};

sf::VertexArray build_base_shape()
{
    static std::vector<sf::Vector2f> const base_shape_pts = {
        { 1, 0.745007 }, { 0.990843, 0.797121 }, { 0.964488, 0.843055 }, { 0.926444, 0.88006 },
        { 0.881982, 0.909156 }, { 0.834055, 0.932167 }, { 0.78415, 0.950527 },
        { 0.733035, 0.965212 }, { 0.681142, 0.976858 }, { 0.628722, 0.985856 },
        { 0.575939, 0.992388 }, { 0.522904, 0.996385 }, { 0.469737, 0.995967 },
        { 0.416667, 0.992442 }, { 0.363817, 0.986467 }, { 0.311315, 0.97797 },
        { 0.259344, 0.966676 }, { 0.208199, 0.952106 }, { 0.158379, 0.933522 },
        { 0.110794, 0.909827 }, { 0.0671886, 0.879483 }, { 0.0308953, 0.840785 },
        { 0.00714522, 0.79345 }, { 0, 0.74097 }, { 0.00228147, 0.687839 }, { 0.00786696, 0.634948 },
        { 0.0162233, 0.582423 }, { 0.0270923, 0.53036 }, { 0.0403624, 0.478855 },
        { 0.0560179, 0.428025 }, { 0.0741149, 0.378012 }, { 0.0947685, 0.329002 },
        { 0.118145, 0.281232 }, { 0.144461, 0.235018 }, { 0.17397, 0.190778 },
        { 0.206948, 0.149064 }, { 0.243654, 0.110598 }, { 0.284256, 0.0762813 },
        { 0.328729, 0.0471695 }, { 0.376725, 0.0243437 }, { 0.427509, 0.00868394 },
        { 0.480036, 0.000609117 }, { 0.533182, 0 }, { 0.585854, 0.00704704 },
        { 0.636818, 0.022083 }, { 0.684873, 0.0447696 }, { 0.729146, 0.0741745 },
        { 0.769237, 0.109082 }, { 0.80514, 0.148296 }, { 0.837087, 0.190802 },
        { 0.865408, 0.23581 }, { 0.890446, 0.282727 }, { 0.912511, 0.331116 },
        { 0.931866, 0.380654 }, { 0.948715, 0.431099 }, { 0.963208, 0.482273 },
        { 0.975431, 0.534037 }, { 0.9854, 0.58628 }, { 0.993032, 0.638916 },
        { 0.998085, 0.69186 }, // closed loop
    };

    sf::VertexArray shape(sf::TriangleFan);
    for (auto const& pt : base_shape_pts) {
        shape.append(sf::Vertex(pt, sf::Color(203, 186, 158)));
    }

    // move the shape so the "base" is centered around (0,0)
    for (std::size_t i = 0; i < shape.getVertexCount(); ++i) {
        shape[i].position -= { 0.5f, 0.74f };
    }

    return shape;
}

sf::VertexArray const& get_base_shape()
{
    static sf::VertexArray const base_shape = build_base_shape();
    return base_shape;
}

Turret::Turret(sf::Vector2f const tile_center, unsigned const tile_size)
    : base_shape(get_base_shape())
    , fire_timer(100)
    , center_of_home_tile(tile_center)
    , barrel_shape(build_circle())
    , barrel_anchor(tile_center)
    , barrel_rotation_speed(0.05f)
    , barrel_ellipse_width(90.f)
    , barrel_ellipse_height(45.f)
    , barrel_angle(0.f)
{
    // place base on the desired tile center
    sf::Transform transform;
    transform.translate(tile_center);
    transform.scale(tile_size * 0.833f, tile_size * 0.833f);

    for (std::size_t i = 0; i < base_shape.getVertexCount(); ++i) {
        base_shape[i].position = transform.transformPoint(base_shape[i].position);
    }

    // HANDLE TURRET BARREL
    sf::Vector2f const barrel_anchor_offset(0.f, 35.f);
    barrel_anchor -= barrel_anchor_offset;
    barrel_shape.setPosition(tile_center + sf::Vector2f(0.f, -10.f));
}

void Turret::update(std::vector<Enemy>& enemies, std::vector<Bullet>& bullets)
{
    float const a = barrel_ellipse_width / 2.f;
    float const b = barrel_ellipse_height / 2.f;

    // Calculate desired angle toward enemy
    float const desired_angle = angle_to(barrel_anchor, enemies[0].shape.getPosition());

    float const desired_delta = shortest_angle_delta(barrel_angle, desired_angle);

    // Clamp rotation speed
    float const frame_delta
        = std::clamp(desired_delta, -barrel_rotation_speed, barrel_rotation_speed);

    barrel_angle += frame_delta;

    // Convert visual angle to parametric ellipse angle
    float t = std::atan2(a * std::sin(barrel_angle), b * std::cos(barrel_angle));

    // Parametric ellipse position
    float const x = barrel_anchor.x + a * std::cos(t);
    float const y = barrel_anchor.y + b * std::sin(t);

    barrel_shape.setPosition(x, y);

    // Fire logic
    fire_timer -= 1;

    float const angle_diff = std::abs(shortest_angle_delta(barrel_angle, desired_angle));

    if (fire_timer <= 0 && angle_diff <= 0.1f) {
        bullets.emplace_back(Bullet(barrel_shape.getPosition(), &enemies[0]));
        fire_timer = 100;
    }
}

#endif
