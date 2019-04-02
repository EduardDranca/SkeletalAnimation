#pragma once

#include <memory>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "BoneShape.h"

class Vertex
{
public:
	~Vertex();
	Vertex(int ID);
	Vertex(int ID, const sf::Vector2f &position);

	void move(const sf::Vector2f & offset);

	void setPosition(const sf::Vector2f & position);

	sf::Vector2f getPosition() const;

	void rotate(float angle, const sf::Vector2f & pivotPoint);

	void setRotation(float angle, const sf::Vector2f & pivotPoint);
	
	void setParent(Vertex *parent);

	void addChild(Vertex *child);

	void removeChild(Vertex *child);

	void setSelected(bool selected);

	bool isSelected() const;

	bool operator==(const Vertex &rhs) const;

	friend class Armature;

private:
	bool selected;

	int ID;

	//the ID of the first vertex in the tree that contains this vertex
	int rootID;

	//the height of this vertex inside the tree
	int height;

	float angle;

	//the BoneShape is the visual representation
	//of the edge between this vertex and its parent
	//TODO: replace with bone (drawable + collision detection)
	std::unique_ptr<BoneShape> drawable;
	
	Vertex *parent;
	std::vector<Vertex*> children;

	sf::Vector2f position;

	bool isRoot() const;

	void updateDrawable();
};

