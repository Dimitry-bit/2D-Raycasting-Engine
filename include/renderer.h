#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

extern sf::RenderWindow* rWindow;

extern bool isDrawHitPoint;
extern bool isDrawHitRay;
extern bool isDrawToInfinity;
extern bool isFollowMouse;

extern int particleStepAngle;

extern sf::Color bgColor;
extern sf::Color bgColor;
extern sf::Color pointColor;
extern sf::Color particleColor;
extern sf::Color rayColor;
extern sf::Color hitRayColor;

void RenderWindow();
void DrawPoint(const sf::Vector2f& point);
void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2);