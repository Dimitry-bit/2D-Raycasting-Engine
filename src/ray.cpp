#include <cmath>

#include "vector2_math.h"
#include "ray.h"
#include "renderer.h"
#include "boundary.h"

ray_t CreateRay(float x1, float y1, float x2, float y2)
{
	sf::Vector2f dir(x2, y2);
	Vector2fNormalize(dir);

	return ray_t{
		{x1, y1},
		dir,
	};
}

ray_t CreateRay(const sf::Vector2f& origin, sf::Vector2f direction)
{
	Vector2fNormalize(direction);
	return ray_t{
		origin,
		direction,
	};
}

ray_t CreateRay(float x1, float y1, float angleInDegree)
{
	sf::Vector2f dir = Vector2fFromAngle(1.0f, angleInDegree);
	return ray_t{
		{x1, y1},
		dir,
	};
}

ray_t CreateRay(const sf::Vector2f& origin, float angleInDegree)
{
	sf::Vector2f dir = Vector2fFromAngle(1.0f, angleInDegree);
	return ray_t{
		origin,
		dir,
	};
}

void RayLookAt(ray_t& ray, const sf::Vector2f& target)
{
	ray.direction.x = target.x - ray.origin.x;
	ray.direction.y = target.y - ray.origin.y;
	Vector2fNormalize(ray.direction);
}

bool RayCast(const ray_t& ray, const boundary_t& boundary, sf::Vector2f& point)
{
	const float x1 = boundary.pA.x;
	const float y1 = boundary.pA.y;
	const float x2 = boundary.pB.x;
	const float y2 = boundary.pB.y;

	const float x3 = ray.origin.x;
	const float y3 = ray.origin.y;
	const float x4 = ray.origin.x + ray.direction.x;
	const float y4 = ray.origin.y + ray.direction.y;

	const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

	if (den == 0)
		return false;

	float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
	float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

	if ((t > 0) && (t < 1) && (u > 0)) {
		point.x = x1 + t * (x2 - x1);
		point.y = y1 + t * (y2 - y1);
		return true;
	}

	return false;
}

void DrawRay(const ray_t& ray)
{
	sf::Vertex line[] = {
		sf::Vertex(ray.origin),
		sf::Vertex(ray.origin + (ray.direction * 50.0f))
	};

	rWindow.draw(line, 2, sf::Lines);
}