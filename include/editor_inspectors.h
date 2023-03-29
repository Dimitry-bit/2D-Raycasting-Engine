#pragma once

#include "editor.h"
#include "boundary.h"
#include "particle.h"

void DrawInspector(editorwindow_t& window);
void DrawBoundaryInspector(boundary_t& boundary);
void DrawParticleInspector(particle_t& particle);
void DrawSceneHierarchy(editorwindow_t& window);
void DrawRenderSettings(editorwindow_t& window);