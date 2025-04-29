#pragma once
#include "../include/Board.h"
#include "../include/Bullet.h"
#include "../include/Button.h"
#include "../include/Enemy.h"
#include "../include/Tower.h"
#include "../include/Turret.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Game_State {

public:
  Game_State(const sf::Vector2i screen_dim, const sf::Vector2u grid_dim,
             const unsigned tile_size_px)
      : board(grid_dim, tile_size_px, screen_dim),
        turret_button(Button(screen_dim)),
        tower(Tower(board.tower_pos, tile_size_px)) {};

  void run(sf::RenderWindow &window, const int target_fps);

private:
  std::vector<Enemy> enemies;
  std::vector<Bullet> bullets;
  std::vector<Turret> turrets;
  Board board;

  Button turret_button;
  Tower tower;

  void update_all(const sf::Vector2i mouse_pos, const bool mouse_clicked);

  void update_enemies();

  void update_bullets();

  void update_turret_placement(const sf::Vector2i &mouse_pos,
                               bool mouse_clicked);

  void update_turrets();

  void draw_all(sf::RenderWindow &window);
};


void Game_State::run(sf::RenderWindow &window, const int target_fps) {
  const sf::Time frame_duration = sf::seconds(1.0f / target_fps);

  sf::Clock globalClock;
  MouseThrottler clickThrottler(sf::milliseconds(200));

  // GAMEPLAY LOOP
  while (window.isOpen()) {
    sf::Clock frame_clock;

    // INPUT
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      window.close();
    }

    const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    const bool mouse_clicked = sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                               clickThrottler.canClick(globalClock);


    update_all(mouse_pos, mouse_clicked);
    draw_all(window);

    window.display();
    sf::Time frameEnd = frame_clock.getElapsedTime();
    if (frameEnd < frame_duration) {
      sf::sleep(frame_duration - frameEnd);
    }
  }
}


void Game_State::update_all(const sf::Vector2i mouse_pos,
                            const bool mouse_clicked) {

  update_enemies();
  update_bullets();
  update_turret_placement(mouse_pos, mouse_clicked);
  update_turrets();

  turret_button.update(mouse_pos, mouse_clicked);
}


void Game_State::update_turret_placement(const sf::Vector2i &mouse_pos,
                                         bool mouse_clicked) {
  board.update_turret_placement_feedback(mouse_pos,
                                         turret_button.turret_selected);

  int idx = board.hovered_tile_idx;
  if (idx < 0)
    return;

  Tile &tile = board.m_tiles[idx];

  if (turret_button.turret_selected) {
    if (tile.m_role == EMPTY && mouse_clicked) {
      turrets.emplace_back(
          Turret(tile.m_top_face.getPosition(), board.m_tile_size));
      tile.m_role = TURRET;
      tile.m_top_face.setFillColor(tile.color_default);
    }
  } else if (tile.m_role == TURRET && mouse_clicked) {
    for (size_t i = 0; i < turrets.size(); i++) {
      if (turrets[i].center_of_home_tile == tile.m_top_face.getPosition()) {
        turrets.erase(turrets.begin() + i);
        tile.m_role = EMPTY;
        break;
      }
    }
  }
}


void Game_State::update_enemies() {
  if (enemies.empty()) {
    enemies.emplace_back(board.spawn_pos, board.tower_pos);
  }

  for (size_t i = 0; i < enemies.size();) {
    enemies[i].update();
    if (enemies[i].health <= 0)
      enemies.erase(enemies.begin() + i);
    else
      ++i;
  }
}


void Game_State::update_bullets() {
  for (size_t i = 0; i < bullets.size();) {
    float dist =
        calc_dist(bullets[i].pos, bullets[i].target->shape.getPosition());
    bullets[i].update(dist);

    if (dist <= bullets[i].target->shape.getRadius() + bullets[i].radius) {
      bullets[i].target->health -= 10.f;
      bullets.erase(bullets.begin() + i);
    } else {
      ++i;
    }
  }
}


void Game_State::update_turrets() {
  for (auto &turret : turrets) {
    turret.update(enemies, bullets);
  }
}


void Game_State::draw_all(sf::RenderWindow &window) {
  window.clear(sf::Color(19, 19, 19));

  board.draw(window);

  for (auto &enemy : enemies) {
    window.draw(enemy.shape);
  }

  for (auto &turret : turrets) {
    if (turret.barrel_shape.getPosition().y > turret.barrel_anchor.y) {
      window.draw(turret.base_shape);
      window.draw(turret.barrel_shape);
    } else {
      window.draw(turret.barrel_shape);
      window.draw(turret.base_shape);
    }
  }

  for (auto &bullet : bullets) {
    window.draw(bullet.shape);
  }

  window.draw(turret_button.shape);
  window.draw(tower.shape);
}
