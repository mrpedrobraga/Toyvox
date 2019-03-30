#pragma once

#include <string.h>
#include <vector>

struct Event {
public:

	//If true, funcking will only trigger again when condition becomes false, then triggers again.
	bool isTriggerOnce;
	bool executed;

	//Conditions when picking objects
	char objectType;

	//When creating an event you'll have to create a function for its test,
	//and another for it's action!
	Event(bool isTOWT) {
		this->isTriggerOnce = isTOWT;
	}

	~Event() {
		delete this;
	}

	//THOSE FUNCTIONS HERE NEED TO BE OVERRIDEN WHEN IMPLEMENTED!
	//They will be defined by the user, and will have a custom body!

	virtual bool testCondition();
	
	virtual void execAction();
};

struct EventHandler
{

	//TODO Add events array
public:
	void testEvents()
	{
		//TODO Cycle trough it's events array and test each condition.
		//If condition is true, execute function!
	}
};

typedef EventHandler EventSheet;