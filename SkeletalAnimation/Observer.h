#pragma once

class Subject;

class Observer
{
public:
	Observer();
	~Observer();

	virtual void update(Subject *subject) = 0;
};

