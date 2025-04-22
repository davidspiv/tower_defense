#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

#include "util.h"

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
    : shape(build_circle(spawn_pos, sf::Color(200, 200, 200), 20.f)),
      speed(.8f), health(100), spawn_pos(spawn_pos), tower_pos(tower_pos) {}


void update_enemies(std::vector<Enemy> &enemies, const sf::Vector2f spawn_pos,
                    const sf::Vector2f tower_pos) {
  for (size_t i = 0; i < enemies.size(); i++) {
    enemies[i].update();
    if (enemies[i].health <= 0) {
      enemies.erase(enemies.begin() + i);
    }
  }

  if (enemies.empty()) {
    Enemy enemy(spawn_pos, tower_pos);
    enemies.emplace_back(enemy);
  }
}

#endif
