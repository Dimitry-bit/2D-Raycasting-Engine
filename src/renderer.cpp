#include <cmath>
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

sf::Color bgColor = sf::Color::Black;
sf::Color pointColor = sf::Color::White;
sf::Color particleColor = sf::Color::White;
sf::Color rayColor = sf::Color(255, 255, 255, 30);
sf::Color hitRayColor = sf::Color(255, 255, 255, 30);

boundary_t b_1 = CreateBoundary(300.0f, 100.0f, 300.0f, 300.0f);

void RenderWindow()
{
	rWindow->clear(bgColor);
	particle_t p_1 = CreateParticle(100.0f, 100.0f, 10.0f, particleStepAngle);

	if (isFollowMouse) {
		ParticleSetPosition(p_1, sf::Vector2f(sf::Mouse::getPosition(*rWindow)));
	}

	DrawParticleHits(p_1, {b_1});

	DrawBoundary(b_1);
	ImGui::SFML::Render(*rWindow);
	rWindow->display();
}

void DrawPoint(const sf::Vector2f& point)
{
	sf::CircleShape circle(5.0f);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setPosition(point);
	circle.setFillColor(pointColor);

	rWindow->draw(circle);
}

void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	sf::Vertex line[] = {
		sf::Vertex(p1, hitRayColor),
		sf::Vertex(p2, hitRayColor)
	};

	sf::RenderStates state;
	state.blendMode = sf::BlendAlpha;

	rWindow->draw(line, 2, sf::Lines, state);
}