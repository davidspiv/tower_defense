#ifndef BUTTON_H
#define BUTTON_H

#include "util.h"

#include <SFML/Graphics.hpp>
#include <cmath>


struct Button {
  sf::Vector2f size;
  sf::Vector2f pos;
  sf::VertexArray shape;
  sf::Color color_norm = sf::Color(189, 181, 155);
  sf::Color color_pressed = sf::Color(112, 107, 92);
  bool tower_selected = false;
  bool was_hovered = false;

  Button(const sf::Vector2i screen_dim)
      : size(100.f, 100.f),
        pos(screen_dim.x / 2.f - size.x / 2.f, screen_dim.y - 150.f) {
    shape = build_primitive_rounded_rect(pos, size, color_norm);
    set_fill_color(color_norm);
  }


  bool is_hovered(const sf::Vector2i &mouse_pos) const {
    return shape.getBounds().contains(static_cast<sf::Vector2f>(mouse_pos));
  }


  void set_fill_color(const sf::Color &color) {
    for (std::size_t i = 0; i < shape.getVertexCount(); ++i) {
      shape[i].color = color;
    }
  }


  void update(const sf::Vector2i &mouse_pos, bool clicked) {
    if (!is_hovered(mouse_pos) && clicked) {
      set_fill_color(color_norm);
      tower_selected = false;

    } else if (tower_selected || (is_hovered(mouse_pos) && clicked)) {

      set_fill_color(color_pressed);
      tower_selected = true;
      was_hovered = true;

    } else if (was_hovered) {
      set_fill_color(color_norm);
    }
  }
};


#endif
