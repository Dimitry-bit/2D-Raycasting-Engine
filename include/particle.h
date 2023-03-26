#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

#include "ray.h"
#include "boundary.h"

struct particle_t {
	sf::CircleShape originCircle;
	std::vector<ray_t> rays;
	sf::Color hitRayColor;
	sf::Color rayColor;
};

particle_t CreateParticle(float x, float y, float radius, int stepAngleInDegree);
particle_t CreateParticle(const sf::Vector2f& origin, float radius, int stepAngleInDegree);
particle_t* CreateParticleAlloc(float x, float y, float radius, int stepAngleInDegree);
particle_t* CreateParticleAlloc(const sf::Vector2f& origin, float radius, int stepAngleInDegree);

void ParticleSetPosition(particle_t& particle, float x, float y);
void ParticleSetPosition(particle_t& particle, const sf::Vector2f& position);
void ParticleSetStepAngle(particle_t& particle, int stepAngle);

std::vector<sf::Vector2f> ParticleCastAllDirections(const particle_t& particle,
                                                    const std::vector<boundary_t>& boundaries);

void DrawParticleHits(const particle_t& particle, const std::vector<boundary_t>& boundaries);

void DrawParticle(const particle_t& particle);