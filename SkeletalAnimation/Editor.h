#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "Armature.h"
#include "Command.h"
#include "InputHandler.h"

class Editor: public Observer
{
public:
	Editor();
	~Editor();

	void run();
private:
	sf::RenderWindow window;

	std::vector<std::unique_ptr<Armature>> armatures;
	
	Armature * currentArmature;

	std::vector<std::unique_ptr<Command>> executedCommands;
	std::unique_ptr<Command> commandToBeExecuted;

	void update();
	void update(Subject * subject);

	std::unique_ptr<Command> createCommand(InputHandler * inputHandler);
	void undoLastCommand();
};