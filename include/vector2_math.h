#pragma once

#include <SFML/Graphics.hpp>

void Vector2fNormalize(sf::Vector2f& vec2f);
sf::Vector2f Vector2fFromAngle(float len, float angleInDegree);
float Vector2fDistance(const sf::Vector2f& v1, const sf::Vector2f& v2);
float Vector2fSquaredDistance(const sf::Vector2f& v1, const sf::Vector2f& v2);