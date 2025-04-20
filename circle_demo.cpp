#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Ellipse Animation");

  // Circle that will move
  sf::CircleShape circle(100.f);
  circle.setFillColor(sf::Color::Red);
  circle.setOrigin(10.f, 10.f); // Center origin

  // Ellipse parameters
  sf::Vector2f center(400.f, 300.f); // Middle of screen
  float width = 300.f;
  float height = 150.f;
  float angle = 0.f;
  float speed = 1.f; // Radians per second

  sf::Clock clock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    float deltaTime = clock.restart().asSeconds();
    angle += speed * deltaTime;

    float a = width / 2.f;
    float b = height / 2.f;
    float x = center.x + a * std::cos(angle);
    float y = center.y + b * std::sin(angle);
    circle.setPosition(x, y);

    window.clear(sf::Color::Black);
    window.draw(circle);
    window.display();
  }

  return 0;
}
