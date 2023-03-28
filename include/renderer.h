#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Time.hpp"

struct pallet_t {
	sf::Color background;
	sf::Color boundary;
	sf::Color point;
	sf::Color particle;
	sf::Color ray;
	sf::Color hitRay;
};

extern sf::RenderWindow* rWindow;

extern bool isDrawHitPoint;
extern bool isDrawHitRay;
extern bool isDrawToInfinity;

extern pallet_t defaultColPallet;

void RenderWindow(sf::Time deltaTime);
void DrawPoint(const sf::Vector2f& point, const sf::Color& color = sf::Color::White);
void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Color& color = sf::Color::White);