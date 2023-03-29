#include <limits>
#include "SFML/Graphics/CircleShape.hpp"

#include "vector2_math.h"
#include "particle.h"
#include "renderer.h"
#include "engine_color.h"

void PopulateRayVector(std::vector<ray_t>& vect, const sf::Vector2f& origin, int nRays);

particle_t CreateParticle(float x, float y, float radius, int nRays)
{
	return CreateParticle({x, y}, radius, nRays);
}

particle_t CreateParticle(const sf::Vector2f& origin, float radius, int nRays)
{
	particle_t particle;
	particle.originCircle.setRadius(radius);
	particle.originCircle.setOrigin(radius, radius);
	particle.originCircle.setPosition(origin);
	particle.originCircle.setFillColor(defaultColPallet.particle);
	particle.nRays = nRays;
	particle.rayColor = defaultColPallet.ray;
	particle.hitRayColor = defaultColPallet.hitRay;
	PopulateRayVector(particle.rays, origin, nRays);
	printf("[INFO][Particle]: Particle created at position (%.2f, %.2f).\n", origin.x, origin.y);

	return particle;
}

particle_t* CreateParticleAlloc(float x, float y, float radius, int nRays)
{
	return CreateParticleAlloc({x, y}, radius, nRays);
}

particle_t* CreateParticleAlloc(const sf::Vector2f& origin, float radius, int nRays)
{
	particle_t* particle = new particle_t;
	particle->originCircle.setRadius(radius);
	particle->originCircle.setOrigin(radius, radius);
	particle->originCircle.setPosition(origin);
	particle->originCircle.setFillColor(defaultColPallet.particle);
	particle->nRays = nRays;
	particle->rayColor = defaultColPallet.ray;
	particle->hitRayColor = defaultColPallet.hitRay;
	PopulateRayVector(particle->rays, origin, nRays);
	printf("[INFO][Particle]: (Alloc) Particle created at position (%.2f, %.2f).\n", origin.x, origin.y);

	return particle;
}

void PopulateRayVector(std::vector<ray_t>& vect, const sf::Vector2f& origin, int nRays)
{
	vect.clear();

	float angle = 360.0f / nRays;

	for (int i = 0; i < nRays; i++) {
		ray_t r = CreateRay(origin, i * angle);
		vect.push_back(r);
	}
}

void ParticleSetPosition(particle_t& particle, float x, float y)
{
	ParticleSetPosition(particle, {x, y});
}

void ParticleSetPosition(particle_t& particle, const sf::Vector2f& position)
{
	particle.originCircle.setPosition(position);
	for (auto& ray: particle.rays) {
		ray.origin = position;
	}
	printf("[INFO][Particle]: Particle position is set to (%.2f, %.2f).\n", position.x, position.y);
}

void ParticleSetNumberOfRays(particle_t& particle, int nRays)
{
	printf("[INFO][Particle]: Particle number of rays is set to %d.\n", nRays);
	UpdateRayCount(int(-particle.rays.size()));
	particle.nRays = nRays;
	PopulateRayVector(particle.rays, particle.originCircle.getPosition(), nRays);
}

void ParticleSetScale(particle_t& particle, const sf::Vector2f& scale)
{
	particle.originCircle.setScale(scale);
	printf("[INFO][Particle]: Particle scale is set to (%.2f, %.2f).\n", scale.x, scale.y);
}

void ParticleDealloc(particle_t* particle)
{
	delete particle;
	printf("[INFO][Particle]: Particle destroyed.\n");
}

std::vector<sf::Vector2f> ParticleCastAllDirections(const particle_t& particle,
                                                    const std::vector<boundary_t>& boundaries)
{
	std::vector<sf::Vector2f> points;
	for (auto& ray: particle.rays) {
		sf::Vector2f closestPoint = ray.origin;
		float minDistance = std::numeric_limits<float>::max();

		for (auto& boundary: boundaries) {
			sf::Vector2f point;
			if (!RayCast(ray, boundary, point)) {
				continue;
			}

			float distance = Vector2fSquaredDistance(ray.origin, point);
			if (distance < minDistance) {
				minDistance = distance;
				closestPoint = point;
			}
		}
		if (closestPoint != ray.origin)
			points.push_back(closestPoint);
	}
	return points;
}

void DrawParticle(const particle_t& particle)
{
	for (auto& ray: particle.rays) {
		DrawRay(ray);
	}

	rWindow->draw(particle.originCircle);
}

void DrawParticleHits(const particle_t& particle, const std::list<boundary_t*>& boundaries)
{
	for (auto& ray: particle.rays) {
		sf::Vector2f closestPoint = ray.origin;
		float minDistance = std::numeric_limits<float>::max();

		for (auto& boundary: boundaries) {
			sf::Vector2f point;
			if (!RayCast(ray, *boundary, point)) {
				continue;
			}

			float distance = Vector2fSquaredDistance(ray.origin, point);
			if (distance < minDistance) {
				minDistance = distance;
				closestPoint = point;
			}
		}

		if (closestPoint != ray.origin) {
			if (isDrawHitPoint) {
				DrawPoint(closestPoint, defaultColPallet.point);
			}
			if (isDrawHitRay) {
				DrawLine(ray.origin, closestPoint, particle.hitRayColor);
			}
		} else {
			DrawRay(ray, particle.rayColor);
		}
	}

	rWindow->draw(particle.originCircle);
}