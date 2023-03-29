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

extern pallet_t defaultColorPalette;