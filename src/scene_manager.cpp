#include "scene_manager.h"

static scene_t scene;
scene_t* const sceneRef = &scene;

selectable_t selectedEntry;

void SceneLoad()
{
	particle_t* p = CreateParticleAlloc(100.0f, 100.0f, 10.0f, particleStepAngle);
	SceneTrackParticle(p);
	SceneTrackBoundary(CreateBoundaryAlloc(300.0f, 100.0f, 300.0f, 300.0f));
	selectedEntry.type = BOUNDARY;
	selectedEntry.data = scene.boundaries.front();
}

void SceneUnload()
{
	for (auto& p: scene.particles)
		ParticleDealloc(p);
	scene.particles.clear();

	for (auto& b: scene.boundaries)
		BoundaryDealloc(b);
	scene.boundaries.clear();
}

void SceneTrackParticle(particle_t* particle)
{
	if (!scene.particles.empty() && std::count(scene.particles.begin(), scene.particles.end(), particle) > 0)
		return;

	scene.particles.push_back(particle);
}

void SceneTrackBoundary(boundary_t* boundary)
{
	if (!scene.boundaries.empty() && std::count(scene.boundaries.begin(), scene.boundaries.end(), boundary) > 0)
		return;

	scene.boundaries.push_back(boundary);
}

particle_t* SceneUnTrackParticle(particle_t* particle)
{
	auto rmIt = std::remove(scene.particles.begin(), scene.particles.end(), particle);
	if (rmIt == scene.particles.end()) {
		return nullptr;
	}

	auto erasedIt = scene.particles.erase(rmIt);
	return *erasedIt;
}

boundary_t* SceneUnTrackBoundary(boundary_t* boundary)
{
	auto rmIt = std::remove(scene.boundaries.begin(), scene.boundaries.end(), boundary);
	if (rmIt == scene.boundaries.end()) {
		return nullptr;
	}

	auto erasedIt = scene.boundaries.erase(rmIt);
	return *erasedIt;
}