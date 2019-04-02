#include "InputHandler.h"



InputHandler::InputHandler()
{
	currentInput.controlHeldDown = false;
	currentInput.shiftHeldDown = false;

	currentInput.keyPressed = -1;

	currentInput.mouseButtonPressed = -1;
	currentInput.mousePosition = sf::Vector2f(0.f, 0.f);
}


InputHandler::~InputHandler()
{
}

void InputHandler::update(const sf::Event & event)
{
	currentInput.type = event.type;

	switch (event.type)
	{
	case (sf::Event::KeyPressed):
	{
		currentInput.keyPressed = event.key.code;

		currentInput.controlHeldDown = event.key.code == sf::Keyboard::LControl;
		currentInput.shiftHeldDown = event.key.code == sf::Keyboard::LShift;

		break;
	}
	case (sf::Event::KeyReleased):
	{
		currentInput.controlHeldDown = !(event.key.code == sf::Keyboard::LControl);
		currentInput.shiftHeldDown = !(event.key.code == sf::Keyboard::LShift);

		break;
	}
	case (sf::Event::MouseButtonPressed):
	{
		currentInput.mouseButtonPressed = event.mouseButton.button;

		break;
	}
	case (sf::Event::MouseMoved):
	{
		currentInput.mousePosition = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		
		break;
	}
	}

	notifyObservers();
}

InputInformation InputHandler::getInput()
{
	return currentInput;
}
