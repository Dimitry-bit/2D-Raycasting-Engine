#include "imgui-SFML.h"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"

#include "renderer.h"

sf::RenderWindow rWindow(sf::VideoMode(640, 480), "HelloSFML");

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

void HandleEvent()
{
	sf::Event event;
	while (rWindow.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(rWindow, event);

		if (event.type == sf::Event::Closed)
			rWindow.close();
	}
}