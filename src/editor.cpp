#include <vector>
#include "imgui.h"
#include "imgui-SFML.h"

#include "editor.h"
#include "editor_inspectors.h"
#include "renderer.h"
#include "engine_color.h"
#include "utility.h"
#include "scene_manager.h"
#include "selection_system.h"

static std::vector<editorwindow_t*> editors;
static bool isDarkTheme = false;
static sf::Time deltaTimeSave;

void EditorCreateAll();

void DrawRenderSettings(editorwindow_t& window);
void DrawMenuBar(editorwindow_t& window);
void DrawAboutMenu(editorwindow_t& window);
void DrawUI();

void EditorSwitchTheme(bool darkTheme);

void EditorInit()
{
	ImGui::SFML::Init(*rWindow);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	EditorCreateAll();
	EditorSwitchTheme(isDarkTheme);
	printf("[INFO][Editor]: Editor initialized successfully.\n");
}

void EditorEvent(sf::Event event)
{
	ImGui::SFML::ProcessEvent(*rWindow, event);
}

editorwindow_t* EditorCreate(const char* name,
                             windowcallback_t callback,
                             sf::Keyboard::Key shortcutKey,
                             bool defaultState)
{
	char* nameAlloc = (char*) malloc(strlen(name) + 1);
	strcpy(nameAlloc, name);

	editorwindow_t* window = (editorwindow_t*) malloc(sizeof(editorwindow_t));

	window->name = nameAlloc;
	window->isOpen = defaultState;
	window->shortcutKey = shortcutKey;
	window->callback = callback;

	editors.push_back(window);

	std::sort(editors.begin(),
	          editors.end(),
	          [](const editorwindow_t* a, const editorwindow_t* b) { return strcmp(a->name, b->name); });

	printf("[INFO][Editor]: %s editor window created successfully.\n", window->name);
	return window;
}

void EditorCreateAll()
{
	EditorCreate("Main Menu Bar", DrawMenuBar);
	EditorCreate("Render Settings", DrawRenderSettings);
	EditorCreate("Inspector", DrawInspector);
	EditorCreate("Scene Hierarchy", DrawSceneHierarchy);
//	EditorCreate("About Menu", DrawAboutMenu);
}

void EditorTick(sf::Time deltaTime)
{
	ImGui::SFML::Update(*rWindow, deltaTime);
	for (auto& editor: editors) {
		if (editor->isOpen && editor->callback) {
			editor->callback(*editor);
		}
	}
	deltaTimeSave = deltaTime;
	DrawUI();
}

void DrawMenuBar(editorwindow_t& window)
{
	if (!ImGui::BeginMainMenuBar()) {
		ImGui::EndMainMenuBar();
		return;
	}

	if (ImGui::BeginMenu("View")) {
		for (auto& editor: editors) {
			ImGui::MenuItem(editor->name, nullptr, &editor->isOpen);
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Tools")) {
		if (ImGui::Checkbox("Dark Theme", &isDarkTheme)) {
			EditorSwitchTheme(isDarkTheme);
		}
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void DrawRenderSettings(editorwindow_t& window)
{
	if (!ImGui::Begin(window.name, &window.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	ImGui::SeparatorText("Draw Options");
	if (ImGui::Checkbox("Draw Hit Ray", &isDrawHitRay)) {
		printf("[INFO][Editor]: Draw Hit Ray %s.\n", isDrawHitRay ? "enabled" : "disabled");
	}
	if (ImGui::Checkbox("Draw Hit Point", &isDrawHitPoint)) {
		printf("[INFO][Editor]: Draw Hit Point %s.\n", isDrawHitPoint ? "enabled" : "disabled");
	}
	if (ImGui::Checkbox("Draw Ray To Infinity", &isDrawToInfinity)) {
		printf("[INFO][Editor]: Draw Ray To Infinity %s.\n", isDrawToInfinity ? "enabled" : "disabled");
	}

	ImGui::SeparatorText("Color Options");
	ImGuiSFMLColorEdit4("BG", defaultColPallet.background, ImGuiColorEditFlags_AlphaPreview);
	ImGuiSFMLColorEdit4("Point", defaultColPallet.point, ImGuiColorEditFlags_AlphaPreview);

	ImGui::SeparatorText("IO Options");
	if (ImGui::Button("Save Scene")) {
		std::string s(FileDialogSaveFile("Scene (*.scene)\0*.scene\0"));
		if (!s.empty()) {
			SelectionDeselectEntry();
			SceneSaveToFile(s.c_str());
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Scene")) {
		std::string s(FileDialogOpenFile("Scene (*.scene)\0*.scene\0"));
		if (!s.empty()) {
			SelectionDeselectEntry();
			SceneLoadFromFile(s.c_str());
		}
	}

	ImGui::End();
}

void DrawIOSettings(editorwindow_t& window)
{
	if (!ImGui::Begin(window.name, &window.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	ImGui::End();
}

void DrawAboutMenu(editorwindow_t& window)
{
	if (!ImGui::Begin(window.name, &window.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	ImGui::Text("2D-RayCaster Engine");
	ImGui::Separator();
	ImGui::Text("By Tony Medhat");

	ImGui::End();
}

void EditorSwitchTheme(bool darkTheme)
{
	if (darkTheme) {
		ImGui::StyleColorsDark();
	} else {
		ImGui::StyleColorsLight();
	}

	printf("[INFO][Editor]: Dark theme %s\n", darkTheme ? "enabled" : "disabled");
}

void DrawUI()
{
	float xOffset = 5.0f;
	float yOffset = 5.0f;
	float yWindowOffset = 15.0f;

	sf::Font font;
	if (!font.loadFromFile("../resources/font/JetBrainsMono-Regular.ttf")) {
		printf("[ERROR][Editor]: Unable to load font.\n");
		return;
	}

	float yPos = rWindow->getSize().y - yWindowOffset;

	sf::Text deltaText;
	deltaText.setFont(font);
	deltaText.setFillColor(sf::Color::White);
	deltaText.setScale(0.5f, 0.5f);
	deltaText.setStyle(sf::Text::Bold);
	deltaText.setString("Framerate: " + std::to_string(deltaTimeSave.asSeconds()) + "s");
	deltaText.setOrigin(0, deltaText.getGlobalBounds().height);
	deltaText.setPosition(xOffset, yPos);

	yPos -= deltaText.getGlobalBounds().height + yOffset;

	sf::Text rayCountText;
	rayCountText.setFont(font);
	rayCountText.setFillColor(sf::Color::White);
	rayCountText.setScale(0.5f, 0.5f);
	rayCountText.setStyle(sf::Text::Bold);
	rayCountText.setString("Rays: " + std::to_string(GetRayCount()));
	rayCountText.setOrigin(0, rayCountText.getGlobalBounds().height);
	rayCountText.setPosition(xOffset, yPos);

	rWindow->draw(deltaText);
	rWindow->draw(rayCountText);
}

void EditorShutdown()
{
	for (auto& editor: editors) {
		printf("[INFO][Editor]: %s editor window freed successfully.\n", editor->name);
		free((void*) editor->name);
		free((void*) editor);
	}

	editors.clear();
	ImGui::SFML::Shutdown();
	printf("[INFO][Editor]: Editor deinitialize successfully.\n");
}

void ImGuiSFMLColorEdit4(const char* label, sf::Color& sfmlColor, ImGuiColorEditFlags colorFlags)
{
	ImVec4 colVec(sfmlColor);
	float colf[4] = {colVec.x, colVec.y, colVec.z, colVec.w};
	if (ImGui::ColorEdit4(label, colf, colorFlags)) {
		printf("[INFO][Editor] %s color changed.\n", label);
	}
	sfmlColor = sf::Color(ImGui::ColorConvertFloat4ToU32({colf[3], colf[2], colf[1], colf[0]}));
}