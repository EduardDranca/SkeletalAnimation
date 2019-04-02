#pragma once

#include <memory>
#include <vector>

#include <SFML/Window/Event.hpp>

#include "Subject.h"
#include "Observer.h"

struct InputInformation
{	
	int type;

	bool shiftHeldDown;
	bool controlHeldDown;

	int keyPressed;

	int mouseButtonPressed;
	sf::Vector2f mousePosition;
};

class InputHandler : public Subject
{
public:
	InputHandler();
	~InputHandler();

	void update(const sf::Event & event);

	InputInformation getInput();

private:
	InputInformation currentInput;

	std::vector<Observer*> observers;
};

