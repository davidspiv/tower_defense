#ifndef TOWER_H
#define TOWER_H

#include "../include/bullet.h"
#include "../include/util.h"

#include <SFML/Graphics.hpp>

struct Tower {
  sf::VertexArray shape;
  Tower(const sf::Vector2f tile_center, const unsigned tile_size);
};


Tower::Tower(const sf::Vector2f tile_center, const unsigned tile_size)
    : shape(sf::TriangleFan) {


  const static std::vector<sf::Vector2f> shape_pts = {
      {0.70025, 0.0746216},  {0.730252, 0.125041},  {0.760254, 0.175461},
      {0.790256, 0.225881},  {0.820258, 0.276301},  {0.85026, 0.326721},
      {0.880262, 0.377141},  {0.910264, 0.427561},  {0.940266, 0.477981},
      {0.970268, 0.5284},    {1.00027, 0.57882},    {1.03027, 0.62924},
      {1.06027, 0.67966},    {1.09028, 0.73008},    {1.04954, 0.75976},
      {0.997061, 0.785999},  {0.944584, 0.812237},  {0.892107, 0.838476},
      {0.83963, 0.864714},   {0.787153, 0.890952},  {0.734676, 0.917191},
      {0.6822, 0.943429},    {0.629723, 0.969668},  {0.577246, 0.995906},
      {0.524769, 1},         {0.472292, 0.973761},  {0.419815, 0.947523},
      {0.367338, 0.921285},  {0.314861, 0.895046},  {0.262384, 0.868808},
      {0.209908, 0.842569},  {0.157431, 0.816331},  {0.104954, 0.790092},
      {0.0524769, 0.763854}, {0, 0.737616},         {0.0288725, 0.687527},
      {0.0588745, 0.637107}, {0.0888764, 0.586687}, {0.118878, 0.536267},
      {0.14888, 0.485847},   {0.178882, 0.435427},  {0.208884, 0.385008},
      {0.238886, 0.334588},  {0.268888, 0.284168},  {0.29889, 0.233748},
      {0.328892, 0.183328},  {0.358894, 0.132908},  {0.388895, 0.0824883},
      {0.437866, 0.052477},  {0.490342, 0.0262386}, {0.54282, 0},
      {0.595296, 0.0221446}, {0.647773, 0.048383},
  };

  // HANDLE TURRET BASE
  for (const auto &pt : shape_pts) {
    shape.append(sf::Vertex(pt, sf::Color(156, 172, 204)));
  }

  // move the shape so the "base" is centered around (0,0)
  for (std::size_t i = 0; i < shape.getVertexCount(); ++i) {
    shape[i].position -= {0.55f, 0.74f};
  }

  // place base on the desired tile center
  const float tile_size_f = static_cast<float>(tile_size);
  sf::Transform transform;
  transform.translate(tile_center);
  transform.scale(tile_size_f * 1.1, tile_size_f * 1.1);

  for (std::size_t i = 0; i < shape.getVertexCount(); ++i) {
    shape[i].position = transform.transformPoint(shape[i].position);
  }
}


#endif
