#include "editor_inspectors.h"

void DrawInspector(editorwindow_t& window)
{
	if (!ImGui::Begin(window.name, &window.isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	ImGui::End();
}

void DrawBoundaryInspector(boundary_t& boundary)
{
	float vec2f[2];

	ImGui::SeparatorText("Transform");

	vec2f[0] = boundary.pA.x;
	vec2f[1] = boundary.pA.y;
	ImGui::DragFloat2("Position A", vec2f);
	boundary.pA.x = vec2f[0];
	boundary.pA.y = vec2f[1];

	vec2f[0] = boundary.pB.x;
	vec2f[1] = boundary.pB.y;
	ImGui::DragFloat2("Position B", vec2f);
	boundary.pB.x = vec2f[0];
	boundary.pB.y = vec2f[1];

	ImGuiSFMLColorEdit4("Color", boundary.color, ImGuiColorEditFlags_AlphaPreview);
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

			particle.originCircle.setScale(vec2);
		}
	}

	int stepAngle = particle.stepAngle;
	if (ImGui::DragInt("Step Angle", &stepAngle, 1, 1, 360, "%d deg")) {
		ParticleSetStepAngle(particle, stepAngle);
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