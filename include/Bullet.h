#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

#include "Enemy.h"
#include "util.h"


struct Bullet {
  sf::Vector2f pos;
  float speed;
  float radius;
  Enemy *target;
  sf::CircleShape shape;

  Bullet(sf::Vector2f tile_center, Enemy *target)
      : pos(tile_center), speed(18.f), radius(4.f), target(target),
        shape(build_circle(pos, sf::Color(49, 45, 38), radius)) {}

  void update(float dist);
};


void Bullet::update(float dist) {
  sf::Vector2f dir = target->shape.getPosition() - pos;
  if (dist != 0) {
    dir /= dist;
  }

  pos += dir * speed;
  shape.setPosition(pos);
}


#endif
