#include "imgui-SFML.h"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"

#include "renderer.h"
#include "editor.h"
#include "scene_manager.h"
#include "input.h"

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
		GameLoop();
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

		switch (event.type) {
			case sf::Event::Closed: {
				rWindow->close();
			}
				break;
			case sf::Event::MouseButtonPressed: {
				SetMouseKeyStatus(event.mouseButton.button, KEY_PRESSED);
			}
				break;
			case sf::Event::KeyPressed:
			case sf::Event::JoystickButtonPressed: {
				SetKeyStatus(event.key.code, KEY_PRESSED);
			}
				break;
			case sf::Event::MouseButtonReleased: {
				SetMouseKeyStatus(event.mouseButton.button, KEY_RELEASED);
			}
				break;
			case sf::Event::KeyReleased:
			case sf::Event::JoystickButtonReleased: {
				SetKeyStatus(event.key.code, KEY_RELEASED);
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