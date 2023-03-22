#include <cmath>
#include "vector2_math.h"

void Vector2fNormalize(sf::Vector2f& vec2f)
{
	const float mag = sqrt((vec2f.x * vec2f.x) + (vec2f.y * vec2f.y));

	if (mag <= 0)
		return;

	vec2f.x /= mag;
	vec2f.y /= mag;
}

sf::Vector2f Vector2fFromAngle(float len, float angleInDegree)
{
	const double angleInRad = angleInDegree * std::numbers::pi / 180;
	float x = len * cos(angleInRad);
	float y = len * sin(angleInRad);

	return sf::Vector2f{x, y};
}

float Vector2fDistance(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
}

float Vector2fSquaredDistance(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
}