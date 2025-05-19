#pragma once

#include "Board.h"
#include "Bullet.h"
#include "Button.h"
#include "Enemy.h"
#include "Tower.h"
#include "Turret.h"
#include "config.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Engine {

public:
    Engine(sf::Vector2i const screen_dim, sf::Vector2u const grid_dim, unsigned const tile_size_px);

    void run(int const target_fps);

private:
    sf::RenderWindow window;

    bool mouse_clicked;
    sf::Vector2i mouse_pos;

    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Turret> turrets;
    Board board;

    Button turret_button;
    Tower tower;

    void input();
    void update();
    void update_enemies();
    void update_bullets();
    void update_turret_placement();
    void update_turrets();

    void draw();
};

void setup_window(sf::RenderWindow& window)
{
    unsigned const width = static_cast<unsigned>(SCREEN_WIDTH);
    unsigned const height = static_cast<unsigned>(SCREEN_HEIGHT);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    window.create(sf::VideoMode(width, height), "Iso Demo", sf::Style::Default, settings);

    auto const desktop = sf::VideoMode::getDesktopMode();
    window.setPosition({ static_cast<int>(desktop.width / 2 - width / 2),
        static_cast<int>(desktop.height / 2 - height / 2) });

    window.setFramerateLimit(TARGET_FPS);

    std::cout << "Anti-Aliasing: " << (window.getSettings().antialiasingLevel ? "ON" : "OFF")
              << "\n";
}

inline Engine::Engine(
    sf::Vector2i const screen_dim, sf::Vector2u const grid_dim, unsigned const tile_size_px)
    : mouse_clicked(false)
    , mouse_pos(sf::Vector2i(0, 0))
    , board(grid_dim, tile_size_px, screen_dim)
    , turret_button(Button(screen_dim))
    , tower(Tower(board.tower_pos, tile_size_px))
{

    setup_window(window);
}

inline void Engine::run(int const target_fps)
{
    // GAMEPLAY LOOP
    while (window.isOpen()) {
        input();
        update();
        draw();
    }
}

inline void Engine::input()
{
    mouse_clicked = false;
    mouse_pos = sf::Mouse::getPosition(window);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) {
            mouse_clicked = true;
        }
    }
}

inline void Engine::update()
{

    update_enemies();
    update_bullets();
    update_turret_placement();
    update_turrets();

    turret_button.update(mouse_pos, mouse_clicked);
}

inline void Engine::update_turret_placement()
{
    board.update_turret_placement_feedback(mouse_pos, turret_button.turret_selected);

    int idx = board.hovered_tile_idx;
    if (idx < 0)
        return;

    Tile& tile = board.m_tiles[idx];

    if (turret_button.turret_selected) {
        if (tile.m_role == EMPTY && mouse_clicked) {
            turrets.emplace_back(Turret(tile.m_top_face.getPosition(), board.m_tile_size));
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

inline void Engine::update_enemies()
{
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

inline void Engine::update_bullets()
{
    for (size_t i = 0; i < bullets.size();) {
        float dist = calc_dist(bullets[i].pos, bullets[i].target->shape.getPosition());
        bullets[i].update(dist);

        if (dist <= bullets[i].target->shape.getRadius() + bullets[i].radius) {
            bullets[i].target->health -= 10.f;
            bullets.erase(bullets.begin() + i);
        } else {
            ++i;
        }
    }
}

inline void Engine::update_turrets()
{
    for (auto& turret : turrets) {
        turret.update(enemies, bullets);
    }
}

inline void Engine::draw()
{
    window.clear(sf::Color(19, 19, 19));

    board.draw(window);

    for (auto& enemy : enemies) {
        window.draw(enemy.shape);
    }

    for (auto& turret : turrets) {
        if (turret.barrel_shape.getPosition().y > turret.barrel_anchor.y) {
            window.draw(turret.base_shape);
            window.draw(turret.barrel_shape);
        } else {
            window.draw(turret.barrel_shape);
            window.draw(turret.base_shape);
        }
    }

    for (auto& bullet : bullets) {
        window.draw(bullet.shape);
    }

    window.draw(turret_button.shape);
    window.draw(tower.shape);

    window.display();
}
