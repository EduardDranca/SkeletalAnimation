#pragma once

#include <queue>

#include <SFML/Graphics.hpp>

#include "Observer.h"
#include "Vertex.h"

//TODO: switch vertex to unique_ptr

class Armature : public Observer
{
public:
	enum
	{
		DEFAULT_MODE = 0,
		EDIT_MODE = 1,
		POSE_MODE = 2,
		MOVE_MODE = 4,
		ROTATE_MODE = 8
	};

	Armature();
	~Armature();

	//returns the new vertices
	std::vector<Vertex*> toggleExtrudeMode();

	void toggleMoveMode();

	void toggleRotateMode();

	//observer pattern update
	void update(Subject *subject);

	//TODO: find a way to remove the reference to the window
	//maybe do the drawing in the editor
	void update(sf::RenderWindow & window);

	//parse all the vertices and select the first one that fits the bill
	//TODO: implement the observer pattern or the command pattern.
	void handleSelection(const sf::Vector2f & mousePosition, bool multiSelection = false);

	std::vector<Vertex*> getSelectedVertices();

	void selectVertex(Vertex *vertex);

	void selectVertices(std::vector<Vertex*> vertices);

	void deselectVertex(Vertex *vertex);

	void clearSelection();

	void deleteChildVertex(Vertex *childVertex);

	void deleteChildrenVertices(std::vector<Vertex*> childrenVertices);

	int getMode();
private:
	static int currentVertexID;

	int mode;

	sf::Vector2f mousePosition;
	sf::Vector2f lastMousePosition;
	sf::Vector2f startingMousePosition;
	sf::Vector2f center;

	std::vector<std::unique_ptr<Vertex>> allVertices;

	std::vector<sf::Drawable*> drawableBones;

	//All the vertices in the armature mapped to the id of the root.
	std::map<int, std::vector<Vertex*>> vertexTrees;

	std::map<int, std::vector<Vertex*>> selectedVertices;

	Vertex* findVertexAtPosition(Vertex *root, const sf::Vector2f & position);

	//Returns the children vertex corresponding to the bone at the position.
	Vertex* findBoneAtPosition(Vertex *root, const sf::Vector2f & position);

	std::unique_ptr<Vertex> createVertex();
	std::unique_ptr<Vertex> createVertex(const sf::Vector2f & position);

	std::vector<sf::Drawable*> getDrawables();
};

