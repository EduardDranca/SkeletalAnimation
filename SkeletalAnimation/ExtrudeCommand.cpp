#include "ExtrudeCommand.h"



ExtrudeCommand::ExtrudeCommand(Armature *armature)
{
	this->armature = armature;
}


ExtrudeCommand::~ExtrudeCommand()
{
}

bool ExtrudeCommand::execute()
{
	if (armature->getMode() == Armature::DEFAULT_MODE)
	{
		selectedVertices = armature->getSelectedVertices();
		newVertices = armature->toggleExtrudeMode();

		return true;
	}

	return false;
}

bool ExtrudeCommand::undo()
{
	armature->deleteChildrenVertices(newVertices);
	armature->selectVertices(selectedVertices);

	return true;
}
