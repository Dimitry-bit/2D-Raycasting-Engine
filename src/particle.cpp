#include <limits>
#include "SFML/Graphics/CircleShape.hpp"

#include "vector2_math.h"
#include "particle.h"
#include "renderer.h"

particle_t CreateParticle(float x, float y, float radius, int stepAngleInDegree)
{
	return CreateParticle({x, y}, radius, stepAngleInDegree);
}

particle_t CreateParticle(const sf::Vector2f& origin, float radius, int stepAngleInDegree)
{
	particle_t particle;
	particle.originCircle.setRadius(radius);
	particle.originCircle.setOrigin(radius, radius);
	particle.originCircle.setPosition(origin);
	particle.originCircle.setFillColor(defaultPallet.particle);
	particle.stepAngle = stepAngleInDegree;

	for (int i = 0; i <= 360; i += stepAngleInDegree) {
		ray_t r = CreateRay(origin, (float) i);
		particle.rays.push_back(r);
	}

	return particle;
}

particle_t* CreateParticleAlloc(float x, float y, float radius, int stepAngleInDegree)
{
	return CreateParticleAlloc({x, y}, radius, stepAngleInDegree);
}

particle_t* CreateParticleAlloc(const sf::Vector2f& origin, float radius, int stepAngleInDegree)
{
	particle_t* particle = new particle_t;
	particle->originCircle.setRadius(radius);
	particle->originCircle.setOrigin(radius, radius);
	particle->originCircle.setPosition(origin);
	particle->originCircle.setFillColor(defaultPallet.particle);
	particle->stepAngle = stepAngleInDegree;

	for (int i = 0; i <= 360; i += stepAngleInDegree) {
		ray_t r = CreateRay(origin, (float) i);
		particle->rays.push_back(r);
	}

	return particle;
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
}

void ParticleSetStepAngle(particle_t& particle, int stepAngle)
{
	particle.rays.clear();
	particle.stepAngle = stepAngle;

	for (int i = 0; i <= 360; i += stepAngle) {
		ray_t r = CreateRay(particle.originCircle.getPosition(), (float) i);
		particle.rays.push_back(r);
	}
}

void ParticleDealloc(particle_t* particle)
{
	delete particle;
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

void DrawParticleHits(const particle_t& particle, const std::vector<boundary_t>& boundaries)
{
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

		if (closestPoint != ray.origin) {
			if (isDrawHitPoint) {
				DrawPoint(closestPoint, defaultPallet.point);
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