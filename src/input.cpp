#include <map>
#include "imgui.h"

#include "input.h"
#include "scene_manager.h"
#include "selection_system.h"
#include "engine_color.h"

enum keystatus_t {
	KEY_OFF = 0,
	KEY_PRESSED,
	KEY_RELEASED,
};

bool isFollowCursor = false;

static std::map<sf::Keyboard::Key, keystatus_t> keyMap;
static std::map<sf::Mouse::Button, keystatus_t> mouseKeyMap;

static boundary_t* b = nullptr;
static int vertexPlacementCount = 0;
static selection_t createObjectOfType = BOUNDARY;

void ClearKeyStatus();
void SetKeyStatus(sf::Keyboard::Key key, keystatus_t status);
void SetMouseKeyStatus(sf::Mouse::Button key, keystatus_t status);

void PlaceParticle();
void PlaceBoundary();
void DestroyBoundary(boundary_t** boundary);
void DestroyParticle(particle_t** particle);

void InputEvent(sf::Event event)
{
	switch (event.type) {
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
		default: break;
	}
}

void GameLoop()
{
	selectable_t selectedEntry = SelectionGetSelectedEntry();

	if (isKeyPressed(sf::Keyboard::Num1)) {
		createObjectOfType = BOUNDARY;
		printf("[INFO][Input]: Instantiate type is set to boundary.\n");
	}

	if (isKeyReleased(sf::Keyboard::Num2)) {
		createObjectOfType = PARTICLE;
		printf("[INFO][Input]: Instantiate type is set to particle.\n");
	}

	if (isKeyPressed(sf::Keyboard::F)) {
		isFollowCursor = !isFollowCursor;
		printf("[INFO][Input]: Follow Cursor %s.\n", isFollowCursor ? "enabled" : "disabled");
	}

	if (isKeyPressed(sf::Keyboard::Delete)) {
		switch (selectedEntry.type) {
			case PARTICLE: {
				DestroyParticle((particle_t**) &selectedEntry.data);
			}
				break;
			case BOUNDARY: {
				DestroyBoundary((boundary_t**) &selectedEntry.data);
			}
				break;
			default: break;
		}

		SelectionDeselectEntry();
	}

	bool isCollidingImGui =
		ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive();

	if (isMousePressed(sf::Mouse::Right) && !isCollidingImGui) {
		switch (createObjectOfType) {
			case PARTICLE: {
				PlaceParticle();
			}
				break;
			case BOUNDARY: {
				PlaceBoundary();
			}
				break;
			default: break;
		}
	}

	if (b && vertexPlacementCount >= 1) {
		b->pB = sf::Vector2f(sf::Mouse::getPosition(*rWindow));
	}

	if (isFollowCursor && selectedEntry.type == PARTICLE && selectedEntry.data) {
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !ImGui::IsAnyItemHovered()) {
			particle_t* p = (particle_t*) selectedEntry.data;
			ParticleSetPosition(*p, sf::Vector2f(sf::Mouse::getPosition(*rWindow)));
		}
	}

	ClearKeyStatus();
}

void DestroyParticle(particle_t** particle)
{
	SceneUnTrackParticle(*particle);
	ParticleDealloc(*particle);
	*particle = nullptr;
}

void PlaceParticle()
{
	sf::Vector2f pos(sf::Mouse::getPosition(*rWindow));
	particle_t* p = CreateParticleAlloc(pos, 10.0f, 10);
	p->rayColor = defaultColPallet.ray;
	SceneTrackParticle(p);
	SelectionSelectEntry(PARTICLE, p);
}

void PlaceBoundary()
{
	if (vertexPlacementCount == 0) {
		if (!b) {
			b = CreateBoundaryAlloc(0, 0, 0, 0);
			b->color = sf::Color::White;
			SceneTrackBoundary(b);
		}

		b->pA = sf::Vector2f(sf::Mouse::getPosition(*rWindow));
		vertexPlacementCount++;

		SelectionSelectEntry(BOUNDARY, b);
	} else {
		b->pB = sf::Vector2f(sf::Mouse::getPosition(*rWindow));
		vertexPlacementCount = 0;
		b = nullptr;
	}
}

void DestroyBoundary(boundary_t** boundary)
{
	if (boundary && *boundary) {
		SceneUnTrackBoundary(*boundary);
		BoundaryDealloc(*boundary);

		if (*boundary == b) {
			vertexPlacementCount = 0;
			b = nullptr;
		}

		*boundary = nullptr;
	}
}

void ClearKeyStatus()
{
	for (auto& key: keyMap)
		key.second = KEY_OFF;
	for (auto& key: mouseKeyMap)
		key.second = KEY_OFF;
}

void SetKeyStatus(sf::Keyboard::Key key, keystatus_t status)
{
	keyMap[key] = status;
}

void SetMouseKeyStatus(sf::Mouse::Button key, keystatus_t status)
{
	mouseKeyMap[key] = status;
}

bool isKeyPressed(sf::Keyboard::Key key)
{
	return (keyMap[key] == KEY_PRESSED);
}

bool isKeyReleased(sf::Keyboard::Key key)
{
	return (keyMap[key] == KEY_RELEASED);
}

bool isMousePressed(sf::Mouse::Button key)
{
	return (mouseKeyMap[key] == KEY_PRESSED);
}

bool isMouseReleased(sf::Mouse::Button key)
{
	return (mouseKeyMap[key] == KEY_RELEASED);
}