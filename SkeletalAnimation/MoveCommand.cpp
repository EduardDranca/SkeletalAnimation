#include "MoveCommand.h"



MoveCommand::MoveCommand(Armature *armature)
{
	this->armature = armature;
}

MoveCommand::~MoveCommand()
{
}

bool MoveCommand::execute()
{
	if (armature->getMode() == Armature::DEFAULT_MODE)
	{
		selectedVertices = armature->getSelectedVertices();

		if (selectedVertices.size() != 0)
		{
			for (int i = 0; i < selectedVertices.size(); i++)
			{
				startingPositions.push_back(selectedVertices[i]->getPosition());
			}

			armature->toggleMoveMode();
		}

		return true;
	}

	return false;
}

bool MoveCommand::undo()
{
	for (int i = 0; i < selectedVertices.size(); i++)
	{
		selectedVertices[i]->setPosition(startingPositions[i]);
	}

	return true;
}