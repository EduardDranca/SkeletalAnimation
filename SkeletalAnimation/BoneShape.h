#pragma once

#include <SFML/Graphics.hpp>

//TODO: redo this (it's shit)
class BoneShape : public sf::Drawable
{
private:
	sf::RectangleShape body;

	std::vector<sf::Vector2f> points;
	std::vector<sf::Vector2f> boundingPolygon;
	std::vector<sf::RectangleShape> rectanglePoints;

	float angle;
	float length;

	int pivotPointIndex;
	bool shouldRecalculatePolygon;

	void recalculateLength();

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;

public:
	BoneShape();
	BoneShape(const sf::Vector2f & firstPoint, const sf::Vector2f &secondPoint);

	void setPivotPoint(int index);
	sf::Vector2f getPivotPoint() const;

	void setPoint(int index, const sf::Vector2f & point);
	void setPointColor(int index, const sf::Color & color);
	sf::Vector2f getPoint(int index) const;

	void move(const sf::Vector2f &offset);
	void move(float xOffset, float yOffset);
	void setPosition(const sf::Vector2f &position);
	void setPosition(float x, float y);
	
	sf::Vector2f getPosition();
	
	void rotate(float angle);
	void setRotation(float angle);

	float getRotation();

	void setOutlineColor(const sf::Color &color);
	void setOutlineThickness(float thickness);

	float getOutlineThickness();

	std::vector<sf::Vector2f> getBoundingPolygon();

	~BoneShape();
};

