#pragma once

#include "editor.h"
#include "boundary.h"
#include "particle.h"

void DrawInspector(editorwindow_t& window);
void DrawBoundaryInspector(boundary_t& boundary);
void DrawParticleInspector(particle_t& particle);

void ImGuiSFMLTransform(sf::Vector2f& position, sf::Vector2f& scale, float& rot);