#include "imgui-SFML.h"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"

#include "renderer.h"
#include "editor.h"
#include "scene_manager.h"

sf::RenderWindow* rWindow;

void HandleEvent();
sf::View GetLetterboxView(sf::View view, int windowWidth, int windowHeight);

int main()
{
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(640, 480), "HelloSFML", sf::Style::Default, contextSettings);
	rWindow = &window;

	ImGui::SFML::Init(window);
	SceneLoad();
	EditorInit();

	sf::Clock deltaClock;
	while (window.isOpen()) {
		HandleEvent();
		ImGui::SFML::Update(window, deltaClock.restart());
		EditorTick();
		RenderWindow();
	}

	EditorShutdown();
	SceneUnload();
	ImGui::SFML::Shutdown();
}

void HandleEvent()
{
	sf::Event event;
	while (rWindow->pollEvent(event)) {
		ImGui::SFML::ProcessEvent(*rWindow, event);

		if (event.type == sf::Event::Closed) {
			rWindow->close();
		} else if (event.type == sf::Event::Resized) {
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			rWindow->setView(sf::View(visibleArea));
		}
	}
}