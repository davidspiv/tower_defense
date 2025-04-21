#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

#include "enemy.h"
#include "util.h"

struct Bullet {
  sf::Vector2f pos;
  float speed;
  float radius;
  Enemy *target;
  sf::CircleShape shape;

  Bullet(sf::Vector2f tile_center, Enemy *target)
      : pos(tile_center), speed(2.f), radius(4.f), target(target),
        shape(build_circle(pos, sf::Color(255, 0, 0), radius)) {}

  void update(float dist) {
    sf::Vector2f dir = target->shape.getPosition() - pos;
    if (dist != 0)
      dir /= dist;

    pos += dir * speed;
    shape.setPosition(pos);
  }
};

void update_bullets(std::vector<Bullet> &bullets) {
  for (size_t i = 0; i < bullets.size(); i++) {
    const float dist =
        calc_dist(bullets[i].pos, bullets[i].target->shape.getPosition());
    bullets[i].update(dist);

    if (dist <= bullets[i].target->shape.getRadius() + bullets[i].radius) {
      bullets[i].target->health -= 10.f;
      bullets.erase(bullets.begin() + i);
      continue;
    }
  }
}

#endif
