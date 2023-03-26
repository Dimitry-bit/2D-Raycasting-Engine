#pragma once

#include <list>

#include "renderer.h"
#include "particle.h"
#include "boundary.h"

enum selections_t {
	NONE,
	PARTICLE,
	BOUNDARY,
};

struct scene_t {
	std::list<particle_t*> particles;
	std::list<boundary_t*> boundaries;
};

struct selectable_t {
	selections_t type;
	void* data;
};

extern scene_t* const sceneRef;
extern selectable_t selectedEntry;

void SceneLoad();
void SceneUnload();

void SceneTrackParticle(particle_t* particle);
void SceneTrackBoundary(boundary_t* boundary);
particle_t* SceneUnTrackParticle(particle_t* particle);
boundary_t* SceneUnTrackBoundary(boundary_t* boundary);