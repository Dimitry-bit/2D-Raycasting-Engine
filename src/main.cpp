#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"

#include "renderer.h"
#include "editor.h"
#include "scene_manager.h"
#include "input.h"

sf::RenderWindow* rWindow;

void HandleEvent();

int main()
{
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(640, 480), "HelloSFML", sf::Style::Default, contextSettings);
	rWindow = &window;

	SceneLoad();
	EditorInit();

	sf::Clock deltaClock;
	while (window.isOpen()) {
		HandleEvent();
		GameLoop();
		RenderWindow(deltaClock.restart());
	}

	EditorShutdown();
	SceneUnload();
}

void HandleEvent()
{
	sf::Event event;
	while (rWindow->pollEvent(event)) {
		EditorEvent(event);
		InputEvent(event);

		switch (event.type) {
			case sf::Event::Closed: {
				rWindow->close();
			}
				break;
			case sf::Event::Resized: {
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				rWindow->setView(sf::View(visibleArea));
			}
				break;
			default: break;
		}
	}
}