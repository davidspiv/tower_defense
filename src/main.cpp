#include "../include/Board.h"
#include "../include/Game_State.h"
#include "../include/Tower.h"
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

  Game_State game_state(SCREEN_DIM);

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
    game_state.update(board, mouse_pos, mouse_clicked);

    // DRAW
    window.clear(sf::Color(19, 19, 19));

    board.draw(window);

    for (auto &enemy : game_state.enemies) {
      window.draw(enemy.shape);
    }

    for (auto &turret : game_state.turrets) {
      if (turret.barrel_shape.getPosition().y > turret.barrel_anchor.y) {
        window.draw(turret.base_shape);
        window.draw(turret.barrel_shape);
      } else {
        window.draw(turret.barrel_shape);
        window.draw(turret.base_shape);
      }
    }

    for (auto &bullet : game_state.bullets) {
      window.draw(bullet.shape);
    }

    window.draw(game_state.turret_button.shape);

    window.draw(tower.shape);

    window.display();
    sf::Time frameEnd = frame_clock.getElapsedTime();
    if (frameEnd < FRAME_DURATION) {
      sf::sleep(FRAME_DURATION - frameEnd);
    }
  }
}
