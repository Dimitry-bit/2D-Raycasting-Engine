#pragma once

#include <list>

#include "renderer.h"
#include "particle.h"
#include "boundary.h"

struct scene_t {
	std::list<particle_t*> particles;
	std::list<boundary_t*> boundaries;
};

extern scene_t* const sceneRef;

void SceneLoad();
void SceneUnload();

void SceneTrackParticle(particle_t* particle);
void SceneTrackBoundary(boundary_t* boundary);
particle_t* SceneUnTrackParticle(particle_t* particle);
boundary_t* SceneUnTrackBoundary(boundary_t* boundary);