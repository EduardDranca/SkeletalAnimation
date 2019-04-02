#include "Editor.h"

#include "ExtrudeCommand.h"
#include "MoveCommand.h"

Editor::Editor()
{
}


Editor::~Editor()
{
}

void Editor::run()
{
	while (window.isOpen())
	{
		update();
	}
}

void Editor::update()
{
	window.clear();
	
	for (int i = 0; i < armatures.size(); i++)
	{
		for (auto drawablePtr : armatures[i]->getDrawables())
		{
			window.draw(*drawablePtr);
		}
	}
	
	window.display();
}

void Editor::update(Subject * subject)
{
	auto inputHandler = dynamic_cast<InputHandler*>(subject);

	if (inputHandler != nullptr)
	{
		auto input = inputHandler->getInput();

		if (input.type == sf::Event::KeyPressed &&
			input.keyPressed == sf::Keyboard::Z &&
			input.controlHeldDown)
		{
			undoLastCommand();
		}
		else
		{
			commandToBeExecuted = createCommand(inputHandler);
			if (commandToBeExecuted != nullptr)
			{
				if (commandToBeExecuted->execute())
				{
					executedCommands.push_back(std::move(commandToBeExecuted));
				}
			}
		}
	}
}

std::unique_ptr<Command> Editor::createCommand(InputHandler * inputHandler)
{
	std::unique_ptr<Command> newCommand = nullptr;
	auto input = inputHandler->getInput();
	switch (input.type)
	{
	case (sf::Event::KeyPressed):
	{
		switch (input.keyPressed)
		{
		case sf::Keyboard::E:
		{
			newCommand = std::make_unique<ExtrudeCommand>(currentArmature);
			break;
		}
		case sf::Keyboard::G:
		{
			newCommand = std::make_unique<MoveCommand>(currentArmature);
			break;
		}
		case sf::Keyboard::R:
		{
			//commandToBeExecuted = std::make_unique<RotateCommand>(currentArmature);
			break;
		}
		}
	}
	case (sf::Event::MouseButtonPressed):
	{
		switch (input.mouseButtonPressed)
		{
		case (sf::Mouse::Left):
		{
			//commandToBeExecuted = std::make_unique<SelectCommand>(currentArmature);
		}
		}

	}
	}

	return newCommand;
}

void Editor::undoLastCommand()
{
	if (!executedCommands.empty())
	{
		auto commandToUndo = std::move(executedCommands.back());
		
		commandToUndo->undo();

		executedCommands.pop_back();
	}
}