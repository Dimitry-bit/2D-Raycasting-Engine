#include "imgui-SFML.h"
#include "SFML/Graphics/CircleShape.hpp"

#include "vector2_math.h"
#include "renderer.h"
#include "boundary.h"
#include "particle.h"

bool isDrawHitPoint = false;
bool isDrawHitRay = true;
bool isDrawToInfinity = true;
bool isFollowMouse = false;

int particleStepAngle = 1;

pallet_t defaultPallet = {
	.background = sf::Color::Black,
	.boundary = sf::Color::White,
	.point = sf::Color::White,
	.particle = sf::Color::White,
	.ray = sf::Color(255, 255, 255, 30),
	.hitRay = sf::Color(255, 255, 255, 30),
};

boundary_t b_1 = CreateBoundary(300.0f, 100.0f, 300.0f, 300.0f);

void RenderWindow()
{
	rWindow->clear(defaultPallet.background);
	particle_t p_1 = CreateParticle(100.0f, 100.0f, 10.0f, particleStepAngle);

	if (isFollowMouse) {
		ParticleSetPosition(p_1, sf::Vector2f(sf::Mouse::getPosition(*rWindow)));
	}

	DrawParticleHits(p_1, {b_1});

	DrawBoundary(b_1);
	ImGui::SFML::Render(*rWindow);
	rWindow->display();
}

void DrawPoint(const sf::Vector2f& point, const sf::Color& color)
{
	sf::CircleShape circle(5.0f);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(point);
	circle.setFillColor(color);

	rWindow->draw(circle);
}

void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Color& color)
{
	sf::Vertex line[] = {
		sf::Vertex(p1, color),
		sf::Vertex(p2, color)
	};

	sf::RenderStates state;
	state.blendMode = sf::BlendAlpha;

	rWindow->draw(line, 2, sf::Lines, state);
}