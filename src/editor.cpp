#include <vector>
#include "imgui.h"

#include "editor.h"
#include "renderer.h"

static std::vector<editorwindow_t*> editors;
static bool isDarkTheme = false;

void EditorInit()
{
	EditorCreateAll();
	EditorSwitchTheme(isDarkTheme);
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

	return window;
}

void EditorCreateAll()
{
	EditorCreate("Main Menu Bar", DrawMenuBar);
	EditorCreate("Render Settings", DrawRenderSettings);
	EditorCreate("Scene Editor", DrawSceneEditorSettings);
	EditorCreate("IO Settings", DrawIOSettings);
//	EditorCreate("About Menu", DrawAboutMenu);
}

void EditorTick()
{
	for (auto& editor: editors) {
		if (editor->isOpen && editor->callback) {
			editor->callback(*editor);
		}
	}
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
	ImGui::Checkbox("Draw Hit Ray", &isDrawHitRay);
	ImGui::Checkbox("Draw Hit Point", &isDrawHitPoint);
	ImGui::Checkbox("Draw Ray To Infinity", &isDrawToInfinity);
	ImGui::Checkbox("Particle Follow Cursor", &isFollowMouse);
	ImGui::DragInt("Step Angle", &particleStepAngle, 1, 1, 360, "%d deg");

	ImGui::SeparatorText("Color Options");
	ImGuiSFMLColorEdit4("BG Color", bgColor, ImGuiColorEditFlags_AlphaPreview);
	ImGuiSFMLColorEdit4("Particle Color", particleColor, ImGuiColorEditFlags_AlphaPreview);
	ImGuiSFMLColorEdit4("Point Color", pointColor, ImGuiColorEditFlags_AlphaPreview);

	static bool enableHitRayCol = false;
	ImGui::Checkbox("Enable Hit Ray Color", &enableHitRayCol);
	ImGuiSFMLColorEdit4("Ray Color", rayColor, ImGuiColorEditFlags_AlphaPreview);
	if (enableHitRayCol) {
		ImGuiSFMLColorEdit4("Hit Ray Color", hitRayColor, ImGuiColorEditFlags_AlphaPreview);
	} else {
		hitRayColor = rayColor;
	}

	ImGui::End();
}

void DrawSceneEditorSettings(editorwindow_t& window)
{

}

void DrawIOSettings(editorwindow_t& window)
{

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
}

void EditorShutdown()
{
	for (auto& editor: editors) {
		free((void*) editor->name);
		free((void*) editor);
	}

	editors.clear();
}

void ImGuiSFMLColorEdit4(const char* label, sf::Color& sfmlColor, ImGuiColorEditFlags colorFlags)
{
	ImVec4 colVec(sfmlColor);
	float colf[4] = {colVec.x, colVec.y, colVec.z, colVec.w};
	ImGui::ColorEdit4(label, colf, colorFlags);
	sfmlColor = sf::Color(ImGui::ColorConvertFloat4ToU32({colf[3], colf[2], colf[1], colf[0]}));
}
