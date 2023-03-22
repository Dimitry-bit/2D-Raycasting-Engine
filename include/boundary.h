#pragma once

#include "SFML/Graphics.hpp"

struct boundary_t {
	sf::Vector2f pA;
	sf::Vector2f pB;
};

boundary_t CreateBoundary(const sf::Vector2f& pA, const sf::Vector2f& pB);
boundary_t CreateBoundary(float x1, float y1, float x2, float y2);

void DrawBoundary(const boundary_t& boundary);