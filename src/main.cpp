#include "imgui-SFML.h"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"

#include "renderer.h"
#include "boundary.h"

sf::RenderWindow rWindow(sf::VideoMode(640, 480), "HelloSFML");
boundary_t b_1 = CreateBoundary(300, 100, 300, 300);

void HandleEvent();

int main()
{
	rWindow.setFramerateLimit(60);
	ImGui::SFML::Init(rWindow);

	sf::Clock deltaClock;
	while (rWindow.isOpen()) {
		HandleEvent();
		ImGui::SFML::Update(rWindow, deltaClock.restart());
		RenderWindow();
	}

	ImGui::SFML::Shutdown();
}

void RenderWindow()
{
	rWindow.clear();
	ImGui::SFML::Render(rWindow);

	DrawBoundary(b_1);
	rWindow.display();
}

void HandleEvent()
{
	sf::Event event;
	while (rWindow.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(rWindow, event);

		if (event.type == sf::Event::Closed)
			rWindow.close();
	}
}
