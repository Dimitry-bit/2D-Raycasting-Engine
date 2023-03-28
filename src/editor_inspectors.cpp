#include "editor_inspectors.h"
#include "scene_manager.h"
#include "input.h"

void DrawInspector(editorwindow_t& window)
{
	if (!ImGui::Begin(window.name, &window.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	switch (selectedEntry.type) {
		case PARTICLE: DrawParticleInspector(*(particle_t*) selectedEntry.data);
			break;
		case BOUNDARY: DrawBoundaryInspector(*(boundary_t*) selectedEntry.data);
			break;
		default: break;
	}

	ImGui::End();
}

void DrawBoundaryInspector(boundary_t& boundary)
{
	float vec2f[2];

	ImGui::SeparatorText("Transform");

	vec2f[0] = boundary.pA.x;
	vec2f[1] = boundary.pA.y;
	if (ImGui::DragFloat2("Position A", vec2f)) {
		BoundarySetPosition(boundary, vec2f[0], vec2f[1], boundary.pB.x, boundary.pB.y);
	}

	vec2f[0] = boundary.pB.x;
	vec2f[1] = boundary.pB.y;
	if (ImGui::DragFloat2("Position B", vec2f)) {
		BoundarySetPosition(boundary, boundary.pA.x, boundary.pA.y, vec2f[0], vec2f[1]);
	}

	ImGui::SeparatorText("Color");
	ImGuiSFMLColorEdit4("Boundary", boundary.color, ImGuiColorEditFlags_AlphaPreview);
}

void DrawParticleInspector(particle_t& particle)
{
	ImGui::SeparatorText("Transform");

	{
		sf::Vector2f vec2 = particle.originCircle.getPosition();
		float vec2f[2] = {vec2.x, vec2.y};
		if (ImGui::DragFloat2("Position", vec2f)) {
			vec2.x = vec2f[0];
			vec2.y = vec2f[1];

			ParticleSetPosition(particle, vec2);
		}
	}

	{
		sf::Vector2f vec2 = particle.originCircle.getScale();
		float vec2f[2] = {vec2.x, vec2.y};
		if (ImGui::DragFloat2("Scale", vec2f)) {
			vec2.x = vec2f[0];
			vec2.y = vec2f[1];

			ParticleSetScale(particle, vec2);
		}
	}

	int nRays = particle.nRays;
	if (ImGui::DragInt("Number of Rays", &nRays, 1, 1, 360)) {
		ParticleSetNumberOfRays(particle, nRays);
	}

	ImGui::SeparatorText("Color");

	sf::Color particleColor = particle.originCircle.getFillColor();
	ImGuiSFMLColorEdit4("Particle", particleColor, ImGuiColorEditFlags_AlphaPreview);
	particle.originCircle.setFillColor(particleColor);
	ImGuiSFMLColorEdit4("Ray", particle.rayColor, ImGuiColorEditFlags_AlphaPreview);

	static bool isHitRayColor = false;
	ImGui::Checkbox("Hit Ray Color", &isHitRayColor);
	if (isHitRayColor) {
		ImGuiSFMLColorEdit4("Hit Ray", particle.hitRayColor, ImGuiColorEditFlags_AlphaPreview);
	} else {
		particle.hitRayColor = particle.rayColor;
	}
}

void DrawSceneHierarchy(editorwindow_t& window)
{
	if (!ImGui::Begin(window.name, &window.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginChild("Scrolling", ImVec2(400.0f, 300.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
		size_t entryCount = 0;
		for (auto& b: sceneRef->boundaries) {
			std::string s("Boundary " + std::to_string(entryCount++));
			bool isSelected = false;

			if (selectedEntry.data == b) {
				isSelected = true;
			}

			if (ImGui::Selectable(s.c_str(), isSelected)) {
				selectedEntry.data = b;
				selectedEntry.type = BOUNDARY;
				isFollowCursor = false;
			}
		}

		entryCount = 0;
		for (auto& p: sceneRef->particles) {
			std::string s("Particle " + std::to_string(entryCount++));
			bool isSelected = false;

			if (selectedEntry.data == p) {
				isSelected = true;
			}

			if (ImGui::Selectable(s.c_str(), isSelected)) {
				selectedEntry.data = p;
				selectedEntry.type = PARTICLE;
				isFollowCursor = false;
			}
		}
	}

	ImGui::EndChild();

	float scrollY = ImGui::GetScrollY();
	float scrollMaxY = ImGui::GetScrollMaxY();
	ImGui::Text("%.0f/%.0f", scrollY, scrollMaxY);

	ImGui::End();
}