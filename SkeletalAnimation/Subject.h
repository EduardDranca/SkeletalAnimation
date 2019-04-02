#pragma once

#include <vector>

#include "Observer.h"

class Subject
{
public:
	Subject();
	~Subject();

	virtual void notifyObservers();
	virtual void registerObserver(Observer *observer);

protected:
	std::vector<Observer*> observers;
};

