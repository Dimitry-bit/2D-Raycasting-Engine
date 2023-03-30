#include <vector>
#include "imgui.h"
#include "imgui-SFML.h"

#include "editor.h"
#include "editor_inspectors.h"
#include "renderer.h"
#include "scene_manager.h"
#include "utility.h"
#include "selection_system.h"

static std::vector<editorwindow_t*> editors;
static bool isDarkTheme = false;
static bool isDrawAboutMenu = false;
static bool isDrawHelp = false;

void EditorCreateAll();
void DrawMenuBar();
void DrawAboutMenu();
void DrawHelp();
void DrawUI(float deltaTime);
void EditorSwitchTheme(bool darkTheme);

void EditorInit()
{
	ImGui::SFML::Init(*rWindow);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableKeyboard;

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
                             bool defaultState)
{
	char* nameAlloc = (char*) malloc(strlen(name) + 1);
	strcpy(nameAlloc, name);

	editorwindow_t* window = (editorwindow_t*) malloc(sizeof(editorwindow_t));

	window->name = nameAlloc;
	window->isOpen = defaultState;
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
	EditorCreate("Render Settings", DrawRenderSettings);
	EditorCreate("Inspector", DrawInspector);
	EditorCreate("Scene Hierarchy", DrawSceneHierarchy);
}

void EditorTick(sf::Time deltaTime)
{
	ImGui::SFML::Update(*rWindow, deltaTime);
	DrawMenuBar();
	DrawAboutMenu();
	DrawHelp();
	DrawUI(deltaTime.asSeconds());
	for (auto& editor: editors) {
		if (editor->isOpen && editor->callback) {
			editor->callback(*editor);
		}
	}
}

void DrawMenuBar()
{
	if (!ImGui::BeginMainMenuBar()) {
		ImGui::EndMainMenuBar();
		return;
	}

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("New")) {
			SelectionDeselectEntry();
			SceneUnload();
		}
		if (ImGui::MenuItem("Open...")) {
			std::string s(FileDialogOpenFile("Scene (*.scene)\0*.scene\0"));
			if (!s.empty()) {
				SelectionDeselectEntry();
				SceneLoadFromFile(s.c_str());
			}
		}
		if (ImGui::MenuItem("Save As...")) {
			std::string s(FileDialogSaveFile("Scene (*.scene)\0*.scene\0"));
			if (!s.empty()) {
				SelectionDeselectEntry();
				SceneSaveToFile(s.c_str());
			}
		}
		ImGui::EndMenu();
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
		ImGui::MenuItem("Help", nullptr, &isDrawHelp);
		ImGui::MenuItem("About", nullptr, &isDrawAboutMenu);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

void DrawHelp()
{
	if (!isDrawHelp) {
		return;
	}

	if (!ImGui::Begin("Help", &isDrawHelp, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	ImGui::Text("%s\n%s\n%s\n%s\n",
	            "Num1 -> boundary",
	            "Num2 -> particle",
	            "Right-Click -> place",
	            "F -> particle follow cursor");

	ImGui::End();
}

void DrawAboutMenu()
{
	if (!isDrawAboutMenu) {
		return;
	}

	if (!ImGui::Begin("About", &isDrawAboutMenu, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	ImGui::Text("Simple 2D Raycaster engine written in C++ using SFML");
	ImGui::Separator();
	ImGui::Text("Made By Tony Medhat");

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

void DrawUI(float deltaTime)
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
	deltaText.setString("Framerate: " + std::to_string(deltaTime) + "s");
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