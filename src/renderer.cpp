#include "imgui-SFML.h"
#include "SFML/Graphics/CircleShape.hpp"

#include "editor.h"
#include "renderer.h"
#include "boundary.h"
#include "particle.h"
#include "scene_manager.h"

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

void RenderWindow(sf::Time deltaTime)
{
	rWindow->clear(defaultPallet.background);

	EditorTick(deltaTime);

	for (auto& p: sceneRef->particles) {
		DrawParticleHits(*p, sceneRef->boundaries);
	}

	for (auto& b: sceneRef->boundaries) {
		DrawBoundary(*b);
	}

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