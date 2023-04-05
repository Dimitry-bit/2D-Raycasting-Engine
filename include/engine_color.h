#pragma once

#include "SFML/Graphics/Color.hpp"

struct pallet_t {
	sf::Color background;
	sf::Color boundary;
	sf::Color point;
	sf::Color particle;
	sf::Color ray;
	sf::Color hitRay;
	sf::Color selection;
};

inline pallet_t defaultColorPalette = {
	.background = sf::Color::Black,
	.boundary = sf::Color::White,
	.point = sf::Color::White,
	.particle = sf::Color::White,
	.ray = sf::Color(255, 255, 255, 70),
	.hitRay = sf::Color(255, 255, 255, 70),
	.selection = sf::Color::Cyan,
};