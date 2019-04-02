#include <iostream>

#include "Vertex.h"

#include "MathUtils.h"

bool Vertex::isRoot() const
{
	return ID == rootID;
}

void Vertex::updateDrawable()
{
	if (drawable != nullptr)
	{
		drawable->setPoint(1, position);
	}
	for (auto child : children)
	{
		child->drawable->setPoint(0, position);
	}
}

Vertex::Vertex(int ID)
{
	this->ID = ID;
	rootID = ID;

	height = 0;
	angle = 0;

	parent = nullptr;
	drawable = nullptr;

	selected = false;
}

Vertex::Vertex(int ID, const sf::Vector2f & position)
{
	this->ID = ID;
	rootID = ID;

	height = 0;
	angle = 0;

	parent = nullptr;
	drawable = nullptr;

	selected = false;

	setPosition(position);
}


Vertex::~Vertex()
{
	std::cout << "Vertex " << ID << " deleted." << std::endl;
}

void Vertex::move(const sf::Vector2f & offset)
{
	position += offset;
	updateDrawable();
}

void Vertex::rotate(float angle, const sf::Vector2f & pivotPoint)
{
	sf::Vector2f offset = position - pivotPoint;

	this->angle += angle;

	sf::Transform t;
	t.rotate(angle);
	offset = t.transformPoint(offset);

	this->setPosition(pivotPoint + offset);
}

void Vertex::setRotation(float angle, const sf::Vector2f & pivotPoint)
{
	rotate(angle - this->angle, pivotPoint);
}

void Vertex::setPosition(const sf::Vector2f & position)
{
	this->position = position;
	updateDrawable();
}

sf::Vector2f Vertex::getPosition() const
{
	return position;
}

void Vertex::setParent(Vertex *parent)
{
	this->parent = parent;
	
	rootID = parent->rootID;
	height = parent->height + 1;

	drawable = std::make_unique<BoneShape>(parent->position, position);

	drawable->setPosition(parent->position);
}

void Vertex::addChild(Vertex *child)
{
	children.push_back(child);
}

void Vertex::removeChild(Vertex * child)
{
	for (auto it = children.begin(); it != children.end(); it++)
	{
		if (*it == child)
		{
			children.erase(it);
			break;
		}
	}
}

void Vertex::setSelected(bool selected)
{
	sf::Color newColor;

	this->selected = selected;

	if (selected == true)
	{
		//change the color of the coresponding point in the BoneShape
		newColor = sf::Color::Blue;
	}
	else
	{
		//change the color back
		newColor = sf::Color::White;
	}

	if (drawable != nullptr)
	{
		//set the second point's color
		drawable->setPointColor(1, newColor);
	}

	for (auto child : children)
	{
		//set the first point's color;
		child->drawable.get()->setPointColor(0, newColor);
	}
}

bool Vertex::isSelected() const
{
	return selected;
}

bool Vertex::operator==(const Vertex & rhs) const
{
	return ID == rhs.ID;
}
