#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "util.h"

struct Enemy {
  sf::Vector2f center;
  sf::CircleShape shape;
  float speed;
  float health;

  Enemy(const sf::Vector2f spawn_pos, const float speed)
      : center(spawn_pos),
        shape(build_circle(spawn_pos, sf::Color(200, 200, 200), 20.f)),
        speed(speed), health(100) {}

  void update() {
    center.x += speed;

    shape.setPosition(center.x - shape.getRadius(),
                      center.y - shape.getRadius());
  }
};

void update_enemies(std::vector<Enemy> &enemies, const sf::Vector2f spawn_pos) {
  for (size_t i = 0; i < enemies.size(); i++) {
    enemies[i].update();
    if (enemies[i].health <= 0) {
      enemies.erase(enemies.begin() + i);
    }
  }

  if (enemies.empty()) {
    Enemy enemy(spawn_pos, 0.1);
    enemies.emplace_back(enemy);
  }
}

#endif
