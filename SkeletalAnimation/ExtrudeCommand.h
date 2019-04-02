#pragma once

#include "Command.h"
#include "Armature.h"

class ExtrudeCommand : public Command
{
public:
	ExtrudeCommand(Armature *armature);
	~ExtrudeCommand();

	bool execute();
	bool undo();
private:
	Armature *armature;

	std::vector<Vertex*> newVertices;
	std::vector<Vertex*> selectedVertices;
};

