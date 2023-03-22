#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

extern sf::RenderWindow rWindow;
extern bool isDrawHitPoint;
extern bool isDrawHitRay;
extern bool isDrawToInfinity;

void RenderWindow();
void DrawPoint(const sf::Vector2f& point);
void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2);