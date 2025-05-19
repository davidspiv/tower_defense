#ifndef BOARD_H
#define BOARD_H

#include "Button.h"
#include "Tile.h"
#include "Turret.h"

#include <SFML/Graphics.hpp>
#include <iostream>

struct Board {
    sf::Vector2u const m_grid_dim;
    unsigned const m_tile_size;
    sf::Vector2i const m_screen_dim;
    std::vector<Tile> m_tiles;
    int hovered_tile_idx;
    sf::Vector2f spawn_pos;
    sf::Vector2f tower_pos;

    Board(sf::Vector2u grid_dim, unsigned tile_size, sf::Vector2i screen_dim);

    sf::Vector2f to_screen_centered_pos(sf::Vector2f const& origin) const;

    // Converts a 2D grid coordinate to a screen-centered isometric position
    sf::Vector2f iso_to_screen(sf::Vector2f const& origin) const;

    // Fills the board with tiles and positions them in isometric layout
    void populate_tiles();

    int get_hovered_tile_idx(sf::Vector2i const mouse_pos);

    void update_turret_placement_feedback(sf::Vector2i const& mouse_pos, bool turret_selected);

    // Draws all the tiles onto the screen
    void draw(sf::RenderWindow& window);
};

Board::Board(sf::Vector2u grid_dim, unsigned const tile_size, sf::Vector2i screen_dim)
    : m_grid_dim(grid_dim)
    , m_tile_size(tile_size)
    , m_screen_dim(sf::Vector2f(screen_dim))
    , hovered_tile_idx(-1)
{
    populate_tiles();

    spawn_pos = m_tiles[5].m_screen_pos;
    tower_pos = m_tiles[9].m_screen_pos;
}

sf::Vector2f Board::to_screen_centered_pos(sf::Vector2f const& origin) const
{
    float iso_board_height = (m_grid_dim.x + m_grid_dim.y - 1) * (m_tile_size / 4.f);
    float x_offset = m_screen_dim.x / 2.f;
    float y_offset = (m_screen_dim.y / 2.f - iso_board_height);

    return origin + sf::Vector2f(x_offset, y_offset);
}

sf::Vector2f Board::iso_to_screen(sf::Vector2f const& iso_pos) const
{
    return sf::Vector2f(iso_pos.x - iso_pos.y, (iso_pos.x + iso_pos.y) / 2.f);
}

void Board::populate_tiles()
{
    m_tiles.reserve(m_grid_dim.x * m_grid_dim.y);

    for (unsigned row = 0; row < m_grid_dim.x; ++row) {
        for (unsigned col = 0; col < m_grid_dim.y; ++col) {
            float x = static_cast<float>(col * m_tile_size);
            float y = static_cast<float>(row * m_tile_size);
            Tile tile(sf::Vector2f(x, y), m_tile_size);

            sf::Vector2f const tile_pos = iso_to_screen(tile.m_origin);
            tile.m_screen_pos = to_screen_centered_pos(tile_pos);
            tile.m_top_face.setPosition(tile.m_screen_pos);
            tile.m_right_face.setPosition(tile.m_screen_pos);
            tile.m_left_face.setPosition(tile.m_screen_pos);

            m_tiles.emplace_back(std::move(tile));
        }
    }
}

int Board::get_hovered_tile_idx(sf::Vector2i const mouse_pos)
{
    if (hovered_tile_idx >= 0) {
        if (m_tiles[hovered_tile_idx].contains(sf::Vector2f(mouse_pos), m_tile_size)) {
            return hovered_tile_idx;
        }
    }

    for (size_t i = 0; i < m_tiles.size(); i++) {
        if (m_tiles[i].contains(sf::Vector2f(mouse_pos), m_tile_size)) {
            return i;
        }
    }

    return -1;
}

void Board::update_turret_placement_feedback(sf::Vector2i const& mouse_pos, bool turret_selected)
{
    int old_idx = hovered_tile_idx;
    hovered_tile_idx = get_hovered_tile_idx(mouse_pos);

    if (old_idx >= 0 && hovered_tile_idx != old_idx) {
        m_tiles[old_idx].m_top_face.setFillColor(m_tiles[old_idx].color_default);
    }

    if (hovered_tile_idx >= 0) {
        Tile& tile = m_tiles[hovered_tile_idx];
        if (turret_selected && tile.m_role == EMPTY) {
            tile.m_top_face.setFillColor(
                tile.color_turret_placement_approved); // highlight buildable
        }
    }
}

void Board::draw(sf::RenderWindow& window)
{
    for (auto& tile : m_tiles) {
        window.draw(tile.m_top_face);
        window.draw(tile.m_right_face);
        window.draw(tile.m_left_face);
    }
}

#endif
