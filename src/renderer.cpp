#include "imgui-SFML.h"
#include "SFML/Graphics/CircleShape.hpp"

#include "renderer.h"
#include "boundary.h"
#include "particle.h"

bool isDrawHitPoint = true;
bool isDrawHitRay = true;
bool isDrawToInfinity = true;

boundary_t b_1 = CreateBoundary(300.0f, 100.0f, 300.0f, 300.0f);
particle_t p_1 = CreateParticle(100.0f, 100.0f, 20.0f, 10);

void RenderWindow()
{
	rWindow.clear();
	ImGui::SFML::Render(rWindow);

	DrawParticle(p_1);
	ParticleSetPosition(p_1, sf::Vector2f(sf::Mouse::getPosition(rWindow)));
	DrawParticleHits(p_1, {b_1});

	DrawBoundary(b_1);
	rWindow.display();
}

void DrawPoint(const sf::Vector2f& point)
{
	sf::CircleShape circle(5.0f);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(point);

	rWindow.draw(circle);
}

void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	sf::Vertex line[] = {
		sf::Vertex(p1),
		sf::Vertex(p2)
	};

	rWindow.draw(line, 2, sf::Lines);
}