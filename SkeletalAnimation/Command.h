#pragma once
class Command
{
public:
	Command();
	~Command();

	//Returns whether the command was executed or not.
	virtual bool execute() = 0;

	//Returns whether the command undo was executed or not.
	virtual bool undo() = 0;
};

