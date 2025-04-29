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
  sf::RenderWindow window;

  std::vector<Enemy> enemies;
  std::vector<Bullet> bullets;
  std::vector<Turret> turrets;
  Board board;

  Button turret_button;
  Tower tower;

  Game_State(const sf::Vector2u grid_dim, const unsigned tile_size_px,
             const sf::Vector2i screen_dim)
      : board(grid_dim, tile_size_px, screen_dim),
        turret_button(Button(screen_dim)),
        tower(Tower(board.tower_pos, tile_size_px)) {};

  void update(const sf::Vector2i mouse_pos, const bool mouse_clicked);

  void draw(sf::RenderWindow &window);
};

void Game_State::update(const sf::Vector2i mouse_pos,
                        const bool mouse_clicked) {
  update_tiles(board.m_tiles, board.m_tile_size, mouse_pos,
               board.hovered_tile_idx);


  if (board.hovered_tile_idx >= 0) {
    Tile &tile = board.m_tiles[board.hovered_tile_idx];

    if (turret_button.tower_selected) {
      if (tile.m_role == EMPTY && mouse_clicked) {
        turrets.emplace_back(
            Turret(tile.m_top_face.getPosition(), board.m_tile_size));
        tile.m_role = TURRET;
      }

      if (tile.contains(sf::Vector2f(mouse_pos), board.m_tile_size)) {
        tile.m_top_face.setFillColor(sf::Color(93, 171, 108));
      }
    } else if (tile.m_role == TURRET && mouse_clicked) {
      for (size_t i = 0; i < turrets.size(); i++) {
        if (turrets[i].center_of_home_tile == tile.m_top_face.getPosition()) {
          turrets.erase(turrets.begin() + i);
          tile.m_role = EMPTY;
          break; // Only remove one
        }
      }
    }
  }

  turret_button.update(mouse_pos, mouse_clicked);

  update_enemies(enemies, board.spawn_pos, board.tower_pos);
  update_bullets(bullets);
  update_turrets(turrets, enemies, bullets);
}

void Game_State::draw(sf::RenderWindow &window) {
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
