#include <iostream>

#include "Armature.h"
#include "MathUtils.h"
#include "InputHandler.h"

int Armature::currentVertexID = 0;

void Armature::handleSelection(const sf::Vector2f & mousePosition, bool multiSelection)
{
	//TODO: check if an edge was selected
	//TODO: check the mode

	mode = DEFAULT_MODE;

	for (auto pair : vertexTrees)
	{
		Vertex *selectedVertex;

		//Each bone is formed between a child vertex and its parent.
		//Therefore each bone corresponds to one child vertex, so they can
		//be considered equivalent
		Vertex *selectedBone;
		
		selectedVertex = findVertexAtPosition(pair.second[0], mousePosition);
		selectedBone = findBoneAtPosition(pair.second[0], mousePosition);

		if (selectedVertex != nullptr)
		{
			//TODO: clean this up if possible
			std::cout << "What?!" << std::endl;
			if (!selectedVertex->isSelected())
			{
				if (!multiSelection)
				{
					clearSelection();
				}

				selectVertex(selectedVertex);
			}
			else
			{
				if (multiSelection)
				{
					deselectVertex(selectedVertex);
				}
				else
				{
					clearSelection();
					selectVertex(selectedVertex);
				}
			}

			return;
		}
		else if (selectedBone != nullptr)
		{
			//TODO: redo this
			handleSelection(selectedBone->getPosition(), multiSelection);
			handleSelection(selectedBone->parent->getPosition(), true);
			return;
		}
	}

	clearSelection();
}

int Armature::getMode()
{
	return mode;
}

Vertex* Armature::findVertexAtPosition(Vertex *root, const sf::Vector2f & position)
{

	float squaredDistance = sam::squaredDistance(root->position, position);

	Vertex *wantedVertex;

	if (squaredDistance <= 121)
	{
		return root;
	}
	else
	{
		for (auto child : root->children)
		{
			wantedVertex = findVertexAtPosition(child, position);

			if (wantedVertex != nullptr)
				return wantedVertex;
		}
	}

	return nullptr;
}

Vertex* Armature::findBoneAtPosition(Vertex *root, const sf::Vector2f & position)
{
	Vertex *wantedVertex;

	if (root->drawable != nullptr && sam::pointInPolygon(position, root->drawable->getBoundingPolygon()))
	{
		return root;
	}
	else
	{
		for (auto child : root->children)
		{
			wantedVertex = findBoneAtPosition(child, position);

			if (wantedVertex != nullptr)
				return wantedVertex;
		}
	}

	return nullptr;
}

void Armature::selectVertex(Vertex *vertex)
{
	//Vertex is set as selected and added to the corresponding vector of
	//selected vertices.
	vertex->setSelected(true);
	selectedVertices[vertex->rootID].push_back(vertex);
}

void Armature::selectVertices(std::vector<Vertex*> vertices)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		selectVertex(vertices[i]);
	}
}

void Armature::deselectVertex(Vertex *vertex)
{
	vertex->setSelected(false);

	auto iterator = std::find(selectedVertices[vertex->rootID].begin(),
		selectedVertices[vertex->rootID].end(),
		vertex);
	selectedVertices[vertex->rootID].erase(iterator);
}

void Armature::clearSelection()
{
	for (auto pair : selectedVertices)
	{
		for (auto vertex : pair.second)
		{
			vertex->setSelected(false);
		}
	}

	selectedVertices.clear();
}

std::vector<Vertex*> Armature::getSelectedVertices()
{
	std::vector<Vertex*> result;

	for (auto pair : selectedVertices)
	{
		for (auto vertex : pair.second)
		{
			result.push_back(vertex);
		}
	}

	return result;
}

void Armature::deleteChildVertex(Vertex *childVertex)
{
	if (childVertex->children.empty())
	{
		Vertex *parentVertex = childVertex->parent;

		if (childVertex->isSelected())
		{
			deselectVertex(childVertex);
		}

		if (parentVertex != nullptr)
		{
			parentVertex->removeChild(childVertex);
		}

		//Search for the vertex in the corresponding tree and then delete it.
		for (auto iterator = vertexTrees[childVertex->rootID].begin();
			iterator != vertexTrees[childVertex->rootID].end();
			iterator++)
		{
			if (*iterator == childVertex)
			{
				vertexTrees[childVertex->rootID].erase(iterator);
				break;
			}
		}

		//Remove the vertex from the armature.
		for (int i = 0; i < allVertices.size(); i++)
		{
			if (allVertices[i].get() == childVertex)
			{
				allVertices.erase(allVertices.begin() + i);
				break;
			}
		}

		//If the parent vertex does not have a parent delete it.
		if (parentVertex != nullptr && parentVertex->isRoot())
		{
			deleteChildVertex(parentVertex);
		}
	}
}

void Armature::deleteChildrenVertices(std::vector<Vertex*> childrenVertices)
{
	for (auto vertex : childrenVertices)
	{
		deleteChildVertex(vertex);
	}
}

std::unique_ptr<Vertex> Armature::createVertex()
{
	std::unique_ptr<Vertex> vertex;

	vertex = std::make_unique<Vertex>(currentVertexID);
	currentVertexID++;

	return vertex;
}

std::unique_ptr<Vertex> Armature::createVertex(const sf::Vector2f & position)
{
	std::unique_ptr<Vertex> vertex;

	vertex = std::make_unique<Vertex>(currentVertexID, position);
	currentVertexID++;

	return vertex;
}

std::vector<sf::Drawable*> Armature::getDrawables()
{
	return drawableBones;
}


//TODO: redo extrusion
std::vector<Vertex*> Armature::toggleExtrudeMode()
{
	mode = MOVE_MODE;

	std::vector<Vertex*> newVertices;

	for (auto pair : selectedVertices)
	{
		int rootID = pair.first;

		for (int i = 0; i < pair.second.size(); i++)
		{
			auto newVertex = createVertex();
			auto currentVertex = selectedVertices[rootID][i];

			newVertex->setPosition(currentVertex->position);
			newVertices.push_back(newVertex.get());

			if (currentVertex->isRoot())
			{
				auto parentVertex = createVertex(currentVertex->position);

				vertexTrees[parentVertex->ID].push_back(parentVertex.get());
				vertexTrees[parentVertex->ID].push_back(newVertex.get());

				newVertex->setParent(parentVertex.get());
				parentVertex->addChild(newVertex.get());

				allVertices.push_back(std::move(parentVertex));
				allVertices.push_back(std::move(newVertex));

				drawableBones.push_back(newVertex->drawable.get());
			}
			else
			{
				newVertex->setParent(currentVertex);
				currentVertex->addChild(newVertex.get());

				vertexTrees[currentVertex->rootID].push_back(newVertex.get());
				allVertices.push_back(std::move(newVertex));

				drawableBones.push_back(newVertex->drawable.get());
			}
		}
	}

	clearSelection();
	selectVertices(newVertices);
	return newVertices;
}

void Armature::toggleMoveMode()
{
	mode = MOVE_MODE;
}

//TODO: redo rotation
void Armature::toggleRotateMode()
{
	//TODO: switch to toggle
	//TODO: calculate center while selecting
	//TODO: rotate single vertex around parent
	int numberOfVertices = 0;

	center = sf::Vector2f(0, 0);

	for (auto pair : selectedVertices)
	{
		for (auto vertex : pair.second)
		{
			center += vertex->position;
		}
		numberOfVertices += pair.second.size();
	}

	if (numberOfVertices > 1)
	{
		mode = ROTATE_MODE;
		center /= (float)numberOfVertices;
		this->startingMousePosition = mousePosition - center;
	}
}

void Armature::update(Subject * subject)
{
	try
	{
		InputHandler *inputHandler = dynamic_cast<InputHandler*>(subject);

		if (inputHandler != nullptr)
		{
			InputInformation input = inputHandler->getInput();

			mousePosition = input.mousePosition;
		}
	}
	catch (std::bad_cast)
	{
		//TODO: some kind of assertion
	}
}

Armature::Armature()
{
	mode = DEFAULT_MODE;

	//Create a single bone.
	std::unique_ptr<Vertex> firstVertex = std::move(createVertex(sf::Vector2f(100, 100)));
	std::unique_ptr<Vertex> secondVertex = std::move(createVertex(sf::Vector2f(100, 20)));

	//Set the second vertex as the child of the first vertex.
	secondVertex->setParent(firstVertex.get());
	firstVertex->addChild(secondVertex.get());

	//Add the vertices to the tree whose root is the first vertex.
	vertexTrees[firstVertex->ID].push_back(firstVertex.get());
	vertexTrees[firstVertex->ID].push_back(secondVertex.get());

	//Move the vertices to the container.
	allVertices.push_back(std::move(firstVertex));
	allVertices.push_back(std::move(secondVertex));

	drawableBones.push_back(secondVertex->drawable.get());
}

Armature::~Armature()
{
}

void Armature::update(sf::RenderWindow & window)
{
	if (mode & MOVE_MODE)
	{
		sf::Vector2f currentMousePosition = mousePosition;

		if (currentMousePosition != lastMousePosition)
		{
			for (auto pair : selectedVertices)
			{
				for (auto vertex : pair.second)
				{
					vertex->move(currentMousePosition - lastMousePosition);
				}
			}
		}
	}

	if (mode & ROTATE_MODE)
	{
		sf::Vector2f relativeMousePosition = mousePosition;

		if (relativeMousePosition != lastMousePosition)
		{
			relativeMousePosition -= center;

			float angle = sam::calculateAngle(relativeMousePosition, startingMousePosition);

			if (sam::crossProduct(relativeMousePosition, startingMousePosition) > 0)
			{
				angle = -angle;
			}

			angle = sam::radToDeg(angle);

			for (auto pair : selectedVertices)
			{
				for (auto vertex : pair.second)
				{
					vertex->setRotation(angle, center);
				}
			}
		}
	}

	lastMousePosition = mousePosition;

	for (auto pair : vertexTrees)
	{
		auto vector = pair.second;

		for (auto vertex : vector)
		{
			if (vertex->drawable != nullptr)
			{
				window.draw(*(vertex->drawable.get()));
			}
		}
	}
}
