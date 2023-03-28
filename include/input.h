#pragma once

#include "SFML/Window.hpp"

#include "scene_manager.h"

void GameLoop();
void InputEvent(sf::Event event);

bool isKeyPressed(sf::Keyboard::Key key);
bool isKeyReleased(sf::Keyboard::Key key);
bool isMousePressed(sf::Mouse::Button key);
bool isMouseReleased(sf::Mouse::Button key);