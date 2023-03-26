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

boundary_t* CreateBoundaryAlloc(float x1, float y1, float x2, float y2)
{
	boundary_t* boundary = new boundary_t;
	boundary->pA.x = x1;
	boundary->pA.y = y1;
	boundary->pB.x = x1;
	boundary->pB.y = y2;
	return boundary;
}

boundary_t* CreateBoundaryAlloc(const sf::Vector2f& pA, const sf::Vector2f& pB)
{
	boundary_t* boundary = new boundary_t;
	boundary->pA = pA;
	boundary->pB = pB;
	return boundary;
}

void BoundaryDealloc(boundary_t* boundary)
{
	delete boundary;
}

void DrawBoundary(const boundary_t& boundary)
{
	sf::Vertex line[] = {
		sf::Vertex(boundary.pA, boundary.color),
		sf::Vertex(boundary.pB, boundary.color)
	};

	rWindow->draw(line, 2, sf::Lines);
}