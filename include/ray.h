#pragma once

#include "SFML/Graphics.hpp"

#include "boundary.h"

struct ray_t {
	sf::Vector2f origin;
	sf::Vector2f direction;
};

ray_t CreateRay(float x1, float y1, float x2, float y2);
ray_t CreateRay(float x1, float y1, float angleInDegree);
ray_t CreateRay(const sf::Vector2f& origin, sf::Vector2f direction);
ray_t CreateRay(const sf::Vector2f& origin, float angleInDegree);

void RayLookAt(ray_t& ray, const sf::Vector2f& target);
bool RayCast(const ray_t& ray, const boundary_t& boundary, sf::Vector2f& point);

void DrawRay(const ray_t& ray);