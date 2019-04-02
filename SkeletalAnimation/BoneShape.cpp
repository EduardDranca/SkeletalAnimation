#include <iostream>

#include "BoneShape.h"
#include "MathUtils.h"


void BoneShape::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(rectanglePoints[0], states);
	target.draw(rectanglePoints[1], states);
	target.draw(body, states);
}

void BoneShape::recalculateLength()
{
	length = sam::distance(points[0], points[1]);
}

BoneShape::BoneShape()
{
	pivotPointIndex = 0;

	shouldRecalculatePolygon = true;

	boundingPolygon.resize(4);

	points.resize(2);

	points[0] = sf::Vector2f(20, 20);
	points[1] = sf::Vector2f(20, 60);

	length = 40;

	rectanglePoints.resize(2);

	rectanglePoints[0] = sf::RectangleShape(sf::Vector2f(8, 8));
	rectanglePoints[0].setOrigin(4, 4);
	rectanglePoints[0].setPosition(20, 20);
	rectanglePoints[0].setOutlineThickness(2);
	rectanglePoints[0].setFillColor(sf::Color::Transparent);

	rectanglePoints[1] = sf::RectangleShape(sf::Vector2f(8, 8));
	rectanglePoints[1].setOrigin(4, 4);
	rectanglePoints[1].setPosition(20, 60);
	rectanglePoints[1].setOutlineThickness(2);
	rectanglePoints[1].setFillColor(sf::Color::Transparent);

	body.setSize(sf::Vector2f(8, 28));
	body.setOrigin(4, 14);
	body.setPosition(20, 40);
	body.setOutlineThickness(2);
	body.setFillColor(sf::Color::Transparent);
}

BoneShape::BoneShape(const sf::Vector2f & firstPoint, const sf::Vector2f & secondPoint)
{
	pivotPointIndex = 0;

	shouldRecalculatePolygon = true;
	boundingPolygon.resize(4);

	rectanglePoints.resize(2);
	points.resize(2);

	points[0] = firstPoint;

	rectanglePoints[0] = sf::RectangleShape(sf::Vector2f(8, 8));
	rectanglePoints[1] = sf::RectangleShape(sf::Vector2f(8, 8));

	rectanglePoints[0].setOrigin(4, 4);
	rectanglePoints[0].setPosition(firstPoint);
	rectanglePoints[0].setOutlineThickness(2);
	rectanglePoints[0].setFillColor(sf::Color::Transparent);

	rectanglePoints[1].setOrigin(4, 4);
	rectanglePoints[1].setPosition(secondPoint);
	rectanglePoints[1].setOutlineThickness(2);
	rectanglePoints[1].setFillColor(sf::Color::Transparent);

	body.setOutlineThickness(2);
	body.setFillColor(sf::Color::Transparent);

	setPoint(1, secondPoint);
}

std::vector<sf::Vector2f> BoneShape::getBoundingPolygon()
{
	if (shouldRecalculatePolygon)
	{
		sf::Transform t;

		//the points have to be translated by the dimensions of the outline
		t = rectanglePoints[0].getTransform();
		
		boundingPolygon[0] = t.transformPoint(rectanglePoints[0].getPoint(2) + sf::Vector2f(2, 2));
		boundingPolygon[1] = t.transformPoint(rectanglePoints[0].getPoint(3) + sf::Vector2f(-2, 2));

		t = rectanglePoints[1].getTransform();

		boundingPolygon[2] = t.transformPoint(rectanglePoints[1].getPoint(0) + sf::Vector2f(-2, -2));
		boundingPolygon[3] = t.transformPoint(rectanglePoints[1].getPoint(1) + sf::Vector2f(2, -2));

		shouldRecalculatePolygon = false;
	}

	return boundingPolygon;
}

void BoneShape::setPivotPoint(int index)
{
	pivotPointIndex = index;
}

sf::Vector2f BoneShape::getPivotPoint() const
{
	return points[pivotPointIndex];
}

void BoneShape::setPoint(int index, const sf::Vector2f & point)
{
	sf::Vector2f normal = sf::Vector2f(0, -1);

	shouldRecalculatePolygon = true;

	points[index] = point;
	rectanglePoints[index].setPosition(point);

	recalculateLength();

	angle = sam::calculateAngle(normal, points[1 - pivotPointIndex]
		                        - points[pivotPointIndex]);
	angle = sam::radToDeg(angle);
	angle = points[1 - pivotPointIndex].x < points[pivotPointIndex].x ? 360 - angle : angle;

	rectanglePoints[0].setRotation(angle);
	rectanglePoints[1].setRotation(angle);

	body.setOrigin(4, (length - 12) / 2);
	body.setPosition((points[0] + points[1]) * 0.5f);
	body.setSize(sf::Vector2f(8, length - 12));
	body.setRotation(angle);
}

void BoneShape::setPointColor(int index, const sf::Color & color)
{
	rectanglePoints[index].setOutlineColor(color);
}

sf::Vector2f BoneShape::getPoint(int index) const
{
	return points.at(index);
}

void BoneShape::setPosition(float x, float y)
{
	sf::Vector2f offset = sf::Vector2f(x, y) - points[pivotPointIndex];

	move(offset);
}

void BoneShape::setPosition(const sf::Vector2f & position)
{
	setPosition(position.x, position.y);
}

void BoneShape::setOutlineThickness(float thickness)
{
}

void BoneShape::setOutlineColor(const sf::Color & color)
{
	rectanglePoints[0].setOutlineColor(color);
	rectanglePoints[1].setOutlineColor(color);
	body.setOutlineColor(color);
}

float BoneShape::getOutlineThickness()
{
	return 0.0f;
}

BoneShape::~BoneShape()
{
}

void BoneShape::move(float xOffset, float yOffset)
{
	rectanglePoints[0].move(xOffset, yOffset);
	rectanglePoints[1].move(xOffset, yOffset);
	body.move(xOffset, yOffset);

	points[0] += sf::Vector2f(xOffset, yOffset);
	points[1] += sf::Vector2f(xOffset, yOffset);
}

void BoneShape::move(const sf::Vector2f & offset)
{
	move(offset.x, offset.y);
}

sf::Vector2f BoneShape::getPosition()
{
	return points[pivotPointIndex];
}

void BoneShape::rotate(float angle)
{
	sf::Transform t;
	t.rotate(angle);
	sf::Vector2f pointOffset = points[1 - pivotPointIndex] - points[pivotPointIndex];
	pointOffset = t.transformPoint(pointOffset);
	pointOffset += points[pivotPointIndex];
	setPoint(1 - pivotPointIndex, pointOffset);
}

void BoneShape::setRotation(float angle)
{
	rotate(angle - this->angle);
}
