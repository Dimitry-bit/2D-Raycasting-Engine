#include "SFML/Graphics.hpp"
#include "boundary.h"
#include "renderer.h"

boundary_t CreateBoundary(float x1, float y1, float x2, float y2)
{
	return boundary_t{
		{x1, y1},
		{x2, y2},
	};
}

boundary_t CreateBoundary(const sf::Vector2f& pA, const sf::Vector2f& pB)
{
	return boundary_t{pA, pB};
}

void DrawBoundary(const boundary_t& boundary)
{
	sf::Vertex line[] = {
		sf::Vertex(boundary.pA),
		sf::Vertex(boundary.pB)
	};

	rWindow->draw(line, 2, sf::Lines);
}