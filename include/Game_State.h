#pragma once
#include "../include/Board.h"
#include "../include/Bullet.h"
#include "../include/Button.h"
#include "../include/Enemy.h"
#include "../include/Turret.h"

#include <vector>

class Game_State {

public:
  std::vector<Enemy> enemies;
  std::vector<Bullet> bullets;
  std::vector<Turret> turrets;

  Button turret_button;

  Game_State(const sf::Vector2i screen_dim)
      : turret_button(Button(screen_dim)) {};

  void update(Board &board, const sf::Vector2i mouse_pos,
              const bool mouse_clicked);
};

void Game_State::update(Board &board, const sf::Vector2i mouse_pos,
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
