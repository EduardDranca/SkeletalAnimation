#include "Subject.h"



Subject::Subject()
{
}


Subject::~Subject()
{
}

void Subject::notifyObservers()
{
	for (auto observer : observers)
	{
		observer->update(this);
	}
}

void Subject::registerObserver(Observer *observer)
{
	observers.push_back(observer);
}