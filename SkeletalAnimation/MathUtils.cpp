#include "MathUtils.h"
#include <math.h>

#include <iostream>

namespace sam
{
	float crossProduct(const sf::Vector2f & v1, const sf::Vector2f & v2)
	{
		return v1.x * v2.y - v2.x * v1.y;
	}

	float dotProduct(const sf::Vector2f & v1, const sf::Vector2f & v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	float distance(const sf::Vector2f &v1, const sf::Vector2f &v2)
	{
		if (v1.x - v2.x == 0)
			return abs(v1.y - v2.y);
		if (v1.y - v2.y == 0)
			return abs(v1.x - v2.x);
		return sqrt(squaredDistance(v1, v2));
	}

	float squaredDistance(const sf::Vector2f &v1, const sf::Vector2f &v2)
	{
		return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
	}

	float calculateAngle(const sf::Vector2f &v1, const sf::Vector2f &v2)
	{
		float lengthV1 = distance(sf::Vector2f(0, 0), v1);
		float lengthV2 = distance(sf::Vector2f(0, 0), v2);

		if (lengthV1 <= 0.0001 || lengthV2 <= 0.0001)
		{
			return 0;
		}

		float cosine = dotProduct(v1, v2) / (lengthV1 * lengthV2);
		if (cosine >= 0.999)
			return 0;
		return acos(cosine);
	}

	float radToDeg(float angle)
	{
		return angle / PI * 180;
	}

	float det2(float mat[2][2])
	{
		return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	}

	sf::Vector2f linesIntersect(const sf::Vector2f & startFirst, const sf::Vector2f & endFirst, const sf::Vector2f & startSecond, const sf::Vector2f & endSecond)
	{
		float mat1[2][2] = { {startFirst.x, startFirst.y}, {endFirst.x, endFirst.y} };
		float mat2[2][2] = { {startSecond.x, startSecond.y}, {endSecond.x, endSecond.y} };

		float firstDeterminant = det2(mat1);
		float secondDeterminant = det2(mat2);

		float firstNumerator[2][2] = { {firstDeterminant, startFirst.x - endFirst.x},
									   {secondDeterminant, startSecond.x - endSecond.x} };
		float secondNumerator[2][2] = { {firstDeterminant, startFirst.y - endFirst.y},
									    {secondDeterminant, startSecond.y - endSecond.y} };;
		float denominator[2][2] = { {firstNumerator[0][1], secondNumerator[0][1]},
									{firstNumerator[1][1], secondNumerator[1][1]} };
		
		float denominatorDet = det2(denominator);

		if (denominatorDet == 0.0)
		{
			return sf::Vector2f(FLT_MAX, FLT_MAX);
		}

		float firstNumeratorDet = det2(firstNumerator);
		float secondNumeratorDet = det2(secondNumerator);

		return sf::Vector2f(firstNumeratorDet / denominatorDet,
			                secondNumeratorDet / denominatorDet);
	}

	bool pointOnSegment(const sf::Vector2f & segmentStart, const sf::Vector2f & segmentEnd, const sf::Vector2f & point)
	{
		sf::Vector2f AB = segmentEnd - segmentStart;
		sf::Vector2f AC = point - segmentStart;
		float cross = crossProduct(AB, AC);
		if (cross <= 0.01 && cross >= -0.01)
		{
			float dotABAB = dotProduct(AB, AB);
			float dotABAC = dotProduct(AB, AC);
			return dotABAC >= 0 && dotABAC <= dotABAB;
		}

		return false;
	}

	bool pointInPolygon(const sf::Vector2f & point, std::vector<sf::Vector2f> polygon)
	{
		int intersectionCount = 0;

		for (int i = 0; i < polygon.size(); i++)
		{
			int j = (i + 1) == polygon.size() ? 0 : i + 1;
			sf::Vector2f intersection = linesIntersect(point, sf::Vector2f(point.x + 1, point.y), polygon[i], polygon[j]);

			if (intersection.x != FLT_MAX && 
				pointOnSegment(polygon[i], polygon[j], intersection) &&
				intersection.x >= point.x)
			{
				intersectionCount++;
			}
		}

		std::cout << "intersectionCount: " << intersectionCount << std::endl;

		return intersectionCount % 2 == 1;
	}
}