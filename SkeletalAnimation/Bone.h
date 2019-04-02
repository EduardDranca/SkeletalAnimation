#pragma once

#include "BoneShape.h"

//TODO: redo this (it's also shit)
//move the mouse "collision" detection in here
class Bone
{
private:

	std::vector<std::shared_ptr<Bone>> connectedChildren;
	std::vector<std::shared_ptr<Bone>> offsetChildren;
	std::shared_ptr<Bone> parent;

	sf::Vector2f pivotPoint;

	void setRotation(float angle, const sf::Vector2f &pivotPoint);
	void rotate(float angle, const sf::Vector2f &pivotPoint, const sf::Transform &transform);

	void setParent(std::shared_ptr<Bone> parent);

	bool selectedPoints[2];
public:
	std::vector<sf::Sprite*> childrenSprites;
	BoneShape shape;
	Bone();
	Bone(const BoneShape &shape);
	~Bone();

	void setPivotPoint(float x, float y);
	void setPivotPoint(const sf::Vector2f &point);
	void movePivotPoint(float xOffset, float yOffset);
	void movePivotPoint(const sf::Vector2f &offset);

	void setSecondaryPoint(float x, float y);
	void setSecondaryPoint(const sf::Vector2f &point);
	void moveSecondaryPoint(float xOffset, float yOffset);
	void moveSecondaryPoint(const sf::Vector2f &offset);

	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f &position);
	void move(float xOffset, float yOffset);
	void move(const sf::Vector2f &offset);
	sf::Vector2f getPosition();

	void setRotation(float angle);
	void rotate(float angle);
	float getRotation();

	void addChildren(std::shared_ptr<Bone> bone, bool offset = false);

	friend class Armature;
};