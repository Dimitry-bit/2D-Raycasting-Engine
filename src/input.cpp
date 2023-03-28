#include <map>
#include "imgui.h"

#include "input.h"
#include "scene_manager.h"

enum keystatus_t {
	KEY_OFF = 0,
	KEY_PRESSED,
	KEY_RELEASED,
};

static std::map<sf::Keyboard::Key, keystatus_t> keyMap;
static std::map<sf::Mouse::Button, keystatus_t> mouseKeyMap;

static boundary_t* b = nullptr;
static int vertexPlacementCount = 0;
static selections_t createObjectOfType = BOUNDARY;

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
	if (isKeyPressed(sf::Keyboard::Num1)) {
		createObjectOfType = BOUNDARY;
	}

	if (isKeyReleased(sf::Keyboard::Num2)) {
		createObjectOfType = PARTICLE;
	}

	if (isKeyPressed(sf::Keyboard::F)) {
		isFollowMouse = !isFollowMouse;
	}

	if (isKeyPressed(sf::Keyboard::Delete)) {
		switch (selectedEntry.type) {
			case PARTICLE: {
				DestroyParticle((particle_t**) &selectedEntry.data);
				selectedEntry.type = NONE;
			}
				break;
			case BOUNDARY: {
				DestroyBoundary((boundary_t**) &selectedEntry.data);
				selectedEntry.type = NONE;
			}
				break;
			default: break;
		};
	}

	if (isMousePressed(sf::Mouse::Left) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)
		&& !ImGui::IsAnyItemHovered()) {
		if (createObjectOfType == BOUNDARY) {
			PlaceBoundary();
		} else if (createObjectOfType == PARTICLE) {
			PlaceParticle();
		}
	}

	if (b && vertexPlacementCount >= 1) {
		b->pB = sf::Vector2f(sf::Mouse::getPosition(*rWindow));
	}

	if (isFollowMouse && selectedEntry.type == PARTICLE && selectedEntry.data) {
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
	p->rayColor = defaultPallet.ray;
	SceneTrackParticle(p);
	selectedEntry.type = PARTICLE;
	selectedEntry.data = p;
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

		selectedEntry.type = BOUNDARY;
		selectedEntry.data = b;
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