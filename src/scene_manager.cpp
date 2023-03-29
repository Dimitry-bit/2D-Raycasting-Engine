#include <string>
#include <cstring>

#include "scene_manager.h"
#include "engine_color.h"

static scene_t scene;
scene_t* const sceneRef = &scene;

static const char* sceneFileExtension = ".scene";

void SceneLoad()
{
	particle_t* p = CreateParticleAlloc(100.0f, 100.0f, 10.0f, 10);
	SceneTrackParticle(p);
	SceneTrackBoundary(CreateBoundaryAlloc(300.0f, 100.0f, 300.0f, 300.0f));
	printf("[INFO][Scene]: Scene loaded successfully.\n");
}

bool SceneLoadFromFile(const char* filePath)
{
	SceneUnload();

	FILE* fp = fopen(filePath, "rb");
	if (!fp) {
		printf("[ERROR][Scene]: Couldn't open %s for reading.\n", filePath);
		return false;
	}
	printf("[INFO][Scene]: %s opened for reading.\n", filePath);

	fread((void*) &defaultColPallet, sizeof(defaultColPallet), 1, fp);

	size_t size;
	fread((void*) &size, sizeof(size), 1, fp);
	for (size_t i = 0; i < size; ++i) {
		boundary_t* boundary = CreateBoundaryAlloc(0.0f, 0.0f, 0.0f, 0.0f);
		fread((void*) boundary, sizeof(*boundary), 1, fp);
		SceneTrackBoundary(boundary);
	}

	fread((void*) &size, sizeof(size), 1, fp);
	for (size_t i = 0; i < size; ++i) {
		sf::Vector2f pos;
		sf::Color circleColor, rayColor, hitRayColor;
		int nRay;
		float radius;
		fread((void*) &pos, sizeof(pos), 1, fp);
		fread((void*) &radius, sizeof(radius), 1, fp);
		fread((void*) &circleColor, sizeof(circleColor), 1, fp);
		fread((void*) &nRay, sizeof(nRay), 1, fp);
		fread((void*) &rayColor, sizeof(rayColor), 1, fp);
		fread((void*) &hitRayColor, sizeof(hitRayColor), 1, fp);
		particle_t* particle = CreateParticleAlloc(pos, radius, nRay);
		SceneTrackParticle(particle);
	}

	fclose(fp);
	return true;
}

bool SceneSaveToFile(const char* filePath)
{
	std::string s(filePath);
	if (!s.ends_with(sceneFileExtension)) {
		s.append(sceneFileExtension);
	}

	FILE* fp = fopen(s.c_str(), "wb");
	if (!fp) {
		printf("[ERROR][Scene]: Couldn't open %s for writing.\n", s.c_str());
		return false;
	}
	printf("[INFO][Scene]: %s opened for writing.\n", s.c_str());

	fwrite((const void*) &defaultColPallet, sizeof(defaultColPallet), 1, fp);

	size_t size = scene.boundaries.size();
	fwrite((const void*) &size, sizeof(size), 1, fp);
	for (auto& boundary: scene.boundaries) {
		fwrite((const void*) boundary, sizeof(*boundary), 1, fp);
	}

	size = scene.particles.size();
	fwrite((const void*) &size, sizeof(size), 1, fp);
	for (auto& particle: scene.particles) {
		fwrite((const void*) &particle->originCircle.getPosition(),
		       sizeof(particle->originCircle.getPosition()), 1, fp);

		float radius = particle->originCircle.getRadius();
		fwrite((const void*) &radius, sizeof(radius), 1, fp);

		fwrite((const void*) &particle->originCircle.getFillColor(),
		       sizeof(particle->originCircle.getFillColor()), 1, fp);

		fwrite((const void*) &particle->nRays, sizeof(particle->nRays), 1, fp);
		fwrite((const void*) &particle->rayColor, sizeof(particle->rayColor), 1, fp);
		fwrite((const void*) &particle->hitRayColor, sizeof(particle->hitRayColor), 1, fp);
	}

	fclose(fp);
	return true;
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