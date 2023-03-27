#pragma once

#include "SFML/Window.hpp"

#include "scene_manager.h"

enum keystatus_t {
	KEY_OFF = 0,
	KEY_PRESSED,
	KEY_RELEASED,
};

void GameLoop();
void PlaceParticle();
void PlaceBoundary();

void ClearKeyStatus();
void SetKeyStatus(sf::Keyboard::Key key, keystatus_t status);
void SetMouseKeyStatus(sf::Mouse::Button key, keystatus_t status);
bool isKeyPressed(sf::Keyboard::Key key);
bool isKeyReleased(sf::Keyboard::Key key);
bool isMousePressed(sf::Mouse::Button key);
bool isMouseReleased(sf::Mouse::Button key);