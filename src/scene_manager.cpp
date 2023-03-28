#include "scene_manager.h"

static scene_t scene;
scene_t* const sceneRef = &scene;

selectable_t selectedEntry;

void SceneLoad()
{
	particle_t* p = CreateParticleAlloc(100.0f, 100.0f, 10.0f, 10);
	SceneTrackParticle(p);
	SceneTrackBoundary(CreateBoundaryAlloc(300.0f, 100.0f, 300.0f, 300.0f));
	selectedEntry.type = BOUNDARY;
	selectedEntry.data = scene.boundaries.front();
	printf("[INFO][Scene]: Scene loaded successfully.\n");
}

void SceneUnload()
{
	for (auto& p: scene.particles)
		ParticleDealloc(p);
	scene.particles.clear();

	for (auto& b: scene.boundaries)
		BoundaryDealloc(b);
	scene.boundaries.clear();

	printf("[INFO][Scene]: Scene unloaded successfully.\n");
}

void SceneTrackParticle(particle_t* particle)
{
	if (!scene.particles.empty() && std::count(scene.particles.begin(), scene.particles.end(), particle) > 0) {
		printf("[WARNING][Scene]: Trying to track an already tracked particle.\n");
		return;
	}

	scene.particles.push_back(particle);
	printf("[INFO][Scene]: Particle added to track list.\n");
}

void SceneTrackBoundary(boundary_t* boundary)
{
	if (!scene.boundaries.empty() && std::count(scene.boundaries.begin(), scene.boundaries.end(), boundary) > 0) {
		printf("[WARNING][Scene]: Trying to track an already tracked boundary.\n");
		return;
	}

	scene.boundaries.push_back(boundary);
	printf("[INFO][Scene]: Boundary added to track list.\n");
}

particle_t* SceneUnTrackParticle(particle_t* particle)
{
	auto rmIt = std::remove(scene.particles.begin(), scene.particles.end(), particle);
	if (rmIt == scene.particles.end()) {
		printf("[WARNING][Scene]: Trying to untrack an already untracked particle.\n");
		return nullptr;
	}

	auto erasedIt = scene.particles.erase(rmIt);
	printf("[INFO][Scene]: Particle removed from track list.\n");
	return *erasedIt;
}

boundary_t* SceneUnTrackBoundary(boundary_t* boundary)
{
	auto rmIt = std::remove(scene.boundaries.begin(), scene.boundaries.end(), boundary);
	if (rmIt == scene.boundaries.end()) {
		printf("[WARNING][Scene]: Trying to untrack an already untracked boundary.\n");
		return nullptr;
	}

	auto erasedIt = scene.boundaries.erase(rmIt);
	printf("[INFO][Scene]: Boundary removed from track list.\n");
	return *erasedIt;
}