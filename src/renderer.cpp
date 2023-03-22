#include "imgui-SFML.h"
#include "SFML/Graphics/CircleShape.hpp"

#include "renderer.h"
#include "boundary.h"

boundary_t b_1 = CreateBoundary(300, 100, 300, 300);

void RenderWindow()
{
	rWindow.clear();
	ImGui::SFML::Render(rWindow);

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