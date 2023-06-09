#pragma once

#include "SFML/Graphics.hpp"

struct boundary_t {
	sf::Vector2f pA;
	sf::Vector2f pB;
	sf::Color color;
};

boundary_t CreateBoundary(const sf::Vector2f& pA, const sf::Vector2f& pB);
boundary_t CreateBoundary(float x1, float y1, float x2, float y2);

boundary_t* CreateBoundaryAlloc(float x1, float y1, float x2, float y2);
boundary_t* CreateBoundaryAlloc(const sf::Vector2f& pA, const sf::Vector2f& pB);
void BoundaryDealloc(boundary_t* boundary);

void BoundarySetPosition(boundary_t& boundary, float x1, float y1, float x2, float y2);
void BoundarySetPosition(boundary_t& boundary, const sf::Vector2f& pA, const sf::Vector2f& pB);

void DrawBoundary(const boundary_t& boundary);