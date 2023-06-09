#pragma once

#include "SFML/Window.hpp"

#include "imgui.h"

typedef void (* windowcallback_t)(struct editorwindow_t& window);

struct editorwindow_t {
	const char* name;
	bool isOpen;
	windowcallback_t callback;
};

void EditorInit();
void EditorShutdown();
void EditorTick(sf::Time deltaTime);
void EditorEvent(sf::Event event);

editorwindow_t* EditorCreate(const char* name,
                             windowcallback_t callback,
                             bool defaultState = true);

void ImGuiSFMLColorEdit4(const char* label, sf::Color& sfmlColor, ImGuiColorEditFlags colorFlags = 0);