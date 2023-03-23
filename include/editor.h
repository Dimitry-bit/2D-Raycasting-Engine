#pragma once

#include "imgui.h"
#include "SFML/Window.hpp"

typedef void (* windowcallback_t)(struct editorwindow_t& window);

struct editorwindow_t {
	const char* name;
	bool isOpen;
	sf::Keyboard::Key shortcutKey;
	windowcallback_t callback;
};

void EditorInit();
void EditorShutdown();
void EditorTick();

editorwindow_t* EditorCreate(const char* name,
                             windowcallback_t callback,
                             sf::Keyboard::Key shortcutKey = sf::Keyboard::Unknown,
                             bool defaultState = true);
void EditorCreateAll();

void DrawRenderSettings(editorwindow_t& window);
void DrawSceneEditorSettings(editorwindow_t& window);
void DrawIOSettings(editorwindow_t& window);
void DrawMenuBar(editorwindow_t& window);
void DrawAboutMenu(editorwindow_t& window);
void DrawEditor();
void DrawDeltaTime();

void EditorSwitchTheme(bool darkTheme);
void ImGuiSFMLColorEdit4(const char* label, sf::Color& sfmlColor, ImGuiColorEditFlags colorFlags = 0);
