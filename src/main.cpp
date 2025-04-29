#include "../include/board.h"
#include "../include/bullet.h"
#include "../include/button.h"
#include "../include/enemy.h"
#include "../include/tile.h"
#include "../include/tower.h"
#include "../include/turret.h"
#include "../include/util.h"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread> // For std::this_thread::sleep_for
#include <vector>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr unsigned BOARD_ROWS = 5;
constexpr unsigned BOARD_COLS = 5;
constexpr unsigned TILE_SIZE_PX = 100;

int main() {
  const sf::Vector2i SCREEN_DIM(SCREEN_WIDTH, SCREEN_HEIGHT);
  const sf::Vector2u GRID_DIM(BOARD_ROWS, BOARD_COLS);

  const int TARGET_FPS = 60;
  const sf::Time FRAME_DURATION = sf::seconds(1.0f / TARGET_FPS);

  // SETUP
  sf::RenderWindow window;
  setup_window(window, SCREEN_DIM);
  Board board(GRID_DIM, TILE_SIZE_PX, SCREEN_DIM);

  sf::Clock globalClock;
  MouseThrottler clickThrottler(sf::milliseconds(200));

  const sf::Vector2f spawn_pos = board.m_tiles[5].m_screen_pos;
  const sf::Vector2f tower_pos = board.m_tiles[9].m_screen_pos;

  std::vector<Enemy> enemies;
  std::vector<Bullet> bullets;
  std::vector<Turret> turrets;

  Button turret_button(SCREEN_DIM);

  const Tower tower(board.m_tiles[9].m_top_face.getPosition(), TILE_SIZE_PX);

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


    // UPDATE
    const int hovered_tile_idx =
        update_tiles(board.m_tiles, board.m_tile_size, mouse_pos);
    update_enemies(enemies, spawn_pos, tower_pos);
    update_bullets(bullets);
    update_turrets(turrets, enemies, bullets);

    if (hovered_tile_idx >= 0) {
      Tile &tile = board.m_tiles[hovered_tile_idx];

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

    // DRAW
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

    window.display();
    sf::Time frameEnd = frame_clock.getElapsedTime();
    if (frameEnd < FRAME_DURATION) {
      sf::sleep(FRAME_DURATION - frameEnd);
    }
  }
}
