#pragma once
#include "Command.h"
#include "Armature.h"


class MoveCommand : public Command
{
public:
	MoveCommand(Armature *armature);
	~MoveCommand();

	bool execute();
	bool undo();
private:
	Armature *armature;

	std::vector<Vertex*> selectedVertices;
	std::vector<sf::Vector2f> startingPositions;
};

