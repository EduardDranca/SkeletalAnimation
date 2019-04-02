#pragma once

#include <vector>

#include "SFML/System/Vector2.hpp"

namespace sam
{
	constexpr auto PI = 3.141592;

	float crossProduct(const sf::Vector2f &v1, const sf::Vector2f &v2);

	float dotProduct(const sf::Vector2f &v1, const sf::Vector2f &v2);
	
	float distance(const sf::Vector2f &v1, const sf::Vector2f &v2);
	
	float squaredDistance(const sf::Vector2f &v1, const sf::Vector2f &v2);
	
	float calculateAngle(const sf::Vector2f &v1, const sf::Vector2f &v2);

	float radToDeg(float angle);

	float det2(float mat[2][2]);

	sf::Vector2f linesIntersect(const sf::Vector2f &startFirst, const sf::Vector2f &endFirst,
						const sf::Vector2f &startSecond, const sf::Vector2f &endSecond);

	bool pointOnSegment(const sf::Vector2f &segmentStart, const sf::Vector2f &segmentEnd, const sf::Vector2f &point);

	bool pointInPolygon(const sf::Vector2f &point, std::vector<sf::Vector2f> polygon);
}
