#ifndef TURRET_H
#define TURRET_H

#include "../include/bullet.h"
#include "../include/util.h"

#include <SFML/Graphics.hpp>

struct Turret {
  sf::VertexArray base_shape;
  const sf::Vector2f origin;
  int fire_timer;

  sf::CircleShape barrel_shape;
  sf::Vector2f barrel_anchor;
  const float barrel_rotation_speed;
  const float barrel_ellipse_width;
  const float barrel_ellipse_height;
  float barrel_angle;

  Turret(const sf::Vector2f tile_center, const unsigned tile_size);

  void update(std::vector<Enemy> &enemies, std::vector<Bullet> &bullets);

  void update();
};


Turret::Turret(const sf::Vector2f tile_center, const unsigned tile_size)
    : base_shape(sf::TriangleFan), origin(tile_center), fire_timer(100),
      barrel_shape(build_circle()), barrel_anchor(tile_center),
      barrel_rotation_speed(0.05f), barrel_ellipse_width(90.f),
      barrel_ellipse_height(45.f), barrel_angle(0.f) {

  const static std::vector<sf::Vector2f> base_shape_pts = {
      {1, 0.74771},           {0.990843, 0.800013},   {0.964488, 0.846115},
      {0.926444, 0.883253},   {0.881982, 0.912455},   {0.834055, 0.935549},
      {0.78415, 0.953976},    {0.733035, 0.968714},   {0.681142, 0.980402},
      {0.628722, 0.989433},   {0.575939, 0.995989},   {0.522904, 1},
      {0.469737, 0.999581},   {0.416667, 0.996043},   {0.363817, 0.990046},
      {0.311315, 0.981519},   {0.259344, 0.970184},   {0.208199, 0.955561},
      {0.158379, 0.93691},    {0.110794, 0.913128},   {0.0671886, 0.882674},
      {0.0308953, 0.843836},  {0.00714522, 0.796329}, {0, 0.743659},
      {0.00228147, 0.690335}, {0.00786696, 0.637252}, {0.0162233, 0.584537},
      {0.0270923, 0.532284},  {0.0403624, 0.480592},  {0.0560179, 0.429578},
      {0.0741149, 0.379384},  {0.0947685, 0.330195},  {0.118145, 0.282252},
      {0.144461, 0.235871},   {0.17397, 0.19147},     {0.206948, 0.149605},
      {0.243654, 0.110999},   {0.284256, 0.0765581},  {0.328729, 0.0473406},
      {0.376725, 0.024432},   {0.427509, 0.00871545}, {0.480036, 0.000611327},
      {0.533182, 0},          {0.585854, 0.00707261}, {0.636818, 0.0221631},
      {0.684873, 0.0449321},  {0.729146, 0.0744437},  {0.769237, 0.109478},
      {0.80514, 0.148834},    {0.837087, 0.191494},   {0.865408, 0.236666},
      {0.890446, 0.283753},   {0.912511, 0.332318},   {0.931866, 0.382035},
      {0.948715, 0.432663},   {0.963208, 0.484023},   {0.975431, 0.535974},
      {0.9854, 0.588407},     {0.993032, 0.641234},   {0.998085, 0.694371},
      {1, 0.74771}, // closed loop
  };

  // HANDLE TURRET BASE
  for (const auto &pt : base_shape_pts) {
    base_shape.append(sf::Vertex(pt, sf::Color(203, 186, 158)));
  }

  // move the shape so the "base" is centered around (0,0)
  for (std::size_t i = 0; i < base_shape.getVertexCount(); ++i) {
    base_shape[i].position -= {.5f, .8f};
  }

  // place base on the desired tile center
  const float tile_size_f = static_cast<float>(tile_size);
  sf::Transform transform;
  transform.translate(tile_center);
  transform.scale(tile_size_f / 1.2f, tile_size_f / 1.2f);

  for (std::size_t i = 0; i < base_shape.getVertexCount(); ++i) {
    base_shape[i].position = transform.transformPoint(base_shape[i].position);
  }

  // HANDLE TURRET BARREL
  const sf::Vector2f barrel_anchor_offset(0.f, 35.f);
  barrel_anchor -= barrel_anchor_offset;
  barrel_shape.setPosition(tile_center + sf::Vector2f(0, -10));
}


void Turret::update(std::vector<Enemy> &enemies, std::vector<Bullet> &bullets) {

  const float a = barrel_ellipse_width / 2.f;
  const float b = barrel_ellipse_height / 2.f;

  // Calculate desired angle toward enemy
  const float desired_angle =
      angle_to(barrel_anchor, enemies[0].shape.getPosition());

  const float desired_delta = shortest_angle_delta(barrel_angle, desired_angle);

  // Clamp rotation speed
  const float frame_delta =
      std::clamp(desired_delta, -barrel_rotation_speed, barrel_rotation_speed);

  barrel_angle += frame_delta;

  // Convert visual angle to parametric ellipse angle
  float t = std::atan2(a * std::sin(barrel_angle), b * std::cos(barrel_angle));

  // Parametric ellipse position
  const float x = barrel_anchor.x + a * std::cos(t);
  const float y = barrel_anchor.y + b * std::sin(t);

  barrel_shape.setPosition(x, y);

  // Fire logic
  fire_timer -= 1;

  const float angle_diff =
      std::abs(shortest_angle_delta(barrel_angle, desired_angle));

  if (fire_timer <= 0 && angle_diff <= 0.1f) {
    bullets.emplace_back(Bullet(barrel_shape.getPosition(), &enemies[0]));
    fire_timer = 100;
  }
}

#endif
