#include "boundary.h"
#include "engine_color.h"
#include "renderer.h"

boundary_t CreateBoundary(float x1, float y1, float x2, float y2)
{
	printf("[INFO][Boundary]: Boundary created at position A(%.2f, %.2f), B(%.2f, %.2f).\n", x1, y1, x2, y2);
	return boundary_t{
		{x1, y1},
		{x2, y2},
		defaultColPallet.boundary,
	};
}

boundary_t CreateBoundary(const sf::Vector2f& pA, const sf::Vector2f& pB)
{
	return boundary_t{pA, pB, defaultColPallet.boundary};
}

boundary_t* CreateBoundaryAlloc(float x1, float y1, float x2, float y2)
{
	boundary_t* boundary = new boundary_t;
	boundary->pA.x = x1;
	boundary->pA.y = y1;
	boundary->pB.x = x1;
	boundary->pB.y = y2;
	boundary->color = defaultColPallet.boundary;
	printf("[INFO][Boundary]: (Alloc) Boundary created at position A(%.2f, %.2f), B(%.2f, %.2f).\n", x1, y1, x2, y2);

	return boundary;
}

boundary_t* CreateBoundaryAlloc(const sf::Vector2f& pA, const sf::Vector2f& pB)
{
	boundary_t* boundary = new boundary_t;
	boundary->pA = pA;
	boundary->pB = pB;
	boundary->color = defaultColPallet.boundary;
	printf("[INFO][Boundary]: (Alloc) Boundary created at position A(%.2f, %.2f), B(%.2f, %.2f).\n",
	       pA.x, pA.y, pB.x, pB.y);

	return boundary;
}

void BoundaryDealloc(boundary_t* boundary)
{
	delete boundary;
	printf("[INFO][Boundary]: Boundary destroyed.\n");
}

void BoundarySetPosition(boundary_t& boundary, float x1, float y1, float x2, float y2)
{
	BoundarySetPosition(boundary, {x1, y1}, {x2, y2});
}

void BoundarySetPosition(boundary_t& boundary, const sf::Vector2f& pA, const sf::Vector2f& pB)
{
	boundary.pA = pA;
	boundary.pB = pB;
	printf("[INFO][Boundary]: Boundary position is set to A(%.2f, %.2f), B(%.2f, %.2f).\n", pA.x, pA.y, pB.x, pB.y);
}

void DrawBoundary(const boundary_t& boundary)
{
	sf::Vertex line[] = {
		sf::Vertex(boundary.pA, boundary.color),
		sf::Vertex(boundary.pB, boundary.color)
	};

	rWindow->draw(line, 2, sf::Lines);
}