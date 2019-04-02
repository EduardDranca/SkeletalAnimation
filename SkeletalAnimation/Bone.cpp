#include "Bone.h"

void Bone::rotate(float angle, const sf::Vector2f &pivotPoint, const sf::Transform &transform)
{
	sf::Vector2f pointOffset;

	shape.rotate(angle);

	pointOffset = this->getPosition() - pivotPoint;
	pointOffset = transform.transformPoint(pointOffset);
	pointOffset += pivotPoint;

	shape.setPosition(pointOffset);
	
	for (auto child : connectedChildren)
	{
		child->rotate(angle, pivotPoint, transform);
	}

	for (auto child : offsetChildren)
	{
		child->rotate(angle, pivotPoint, transform);
	}

	for (auto sprite : childrenSprites)
	{
		sprite->rotate(angle);
		sprite->setPosition(getPosition());
	}
}

void Bone::setParent(std::shared_ptr<Bone> parent)
{
	this->parent = parent;
}

Bone::Bone()
{
	pivotPoint = shape.getPoint(0);
}

Bone::Bone(const BoneShape & shape)
{
	this->shape.setPoint(0, shape.getPoint(0));
	this->shape.setPoint(1, shape.getPoint(1));
}

Bone::~Bone()
{
}

void Bone::setPosition(float x, float y)
{
	move(sf::Vector2f(x, y) - pivotPoint);
}

void Bone::setPosition(const sf::Vector2f & position)
{
	move(position - pivotPoint);
}

void Bone::move(float xOffset, float yOffset)
{
	shape.move(xOffset, yOffset);
	pivotPoint = shape.getPivotPoint();
}

void Bone::move(const sf::Vector2f & offset)
{
	move(offset.x, offset.y);
}

sf::Vector2f Bone::getPosition()
{
	return shape.getPosition();
}

void Bone::rotate(float angle)
{
	sf::Transform transform;
	transform.rotate(angle);
	shape.rotate(angle);

	for (auto child : connectedChildren)
	{
		child->rotate(angle, getPosition(), transform);
	}

	for (auto child : offsetChildren)
	{
		child->rotate(angle, getPosition(), transform);
	}

	for (auto sprite : childrenSprites)
	{
		sprite->rotate(angle);
		sprite->setPosition(getPosition());
	}
}

void Bone::addChildren(std::shared_ptr<Bone> bone, bool offset)
{
	if (!offset)
	{
		connectedChildren.push_back(bone);
		bone->setParent(bone);
	}
	else
	{
		offsetChildren.push_back(bone);
		bone->setParent(bone);
	}
}
