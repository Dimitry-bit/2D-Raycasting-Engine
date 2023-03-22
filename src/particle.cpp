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

	for (int i = 0; i <= 360; i += stepAngleInDegree) {
		ray_t r = CreateRay(origin, (float) i);
		particle.rays.push_back(r);
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

	rWindow.draw(particle.originCircle);
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
				DrawPoint(closestPoint);
			}
			if (isDrawHitRay) {
				DrawLine(ray.origin, closestPoint);
			}
		} else {
			DrawRay(ray);
		}
	}

	rWindow.draw(particle.originCircle);
}