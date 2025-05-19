#pragma once
#include "util.h"

#include <SFML/Graphics.hpp>


struct Enemy {
private:
public:
  sf::CircleShape shape;
  float speed;
  float health;
  sf::Vector2f spawn_pos;
  sf::Vector2f tower_pos;

  Enemy(const sf::Vector2f spawn_pos, const sf::Vector2f tower_pos);

  void update() {
    sf::Vector2f dir = tower_pos - shape.getPosition(); // reversed direction
    float dist = calc_dist(shape.getPosition(), tower_pos);

    if (dist < 1.f) {
      return;
    }

    dir /= dist; // normalize

    shape.setPosition(shape.getPosition() + dir * speed);
  }
};


Enemy::Enemy(const sf::Vector2f spawn_pos, const sf::Vector2f tower_pos)
    : shape(build_circle(spawn_pos, sf::Color(240, 200, 78), 20.f)), speed(.8f),
      health(100), spawn_pos(spawn_pos), tower_pos(tower_pos) {}

