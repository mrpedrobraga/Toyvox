#pragma once
#include <math.h>
#include <glm/glm.hpp>
#include <string.h>
#include <vector>
#include <stdio.h>
#include "events.h"

using namespace glm;
using namespace std;

class VoxelModel
{
	//TODO Functions to import .tvx models
	//Transformation matrix
};

typedef VoxelModel VoxelAnimFrame;

class Object {
protected:
	vec3 position;
	vec3 scale;
	vec3 rotation;
	//TODO use "mat4x4 LocRotScale;" instead of all those vec3(s).

	bool isVisible;
	char *objectTypeTag;
public:
	//Getters, setters and adders.
	vec3 getPosition() { return position; }

	void setPosition(vec3 position) { this->position = position; }

	void move(vec3 amount) { this->position += amount; }

	vec3 getScale() { return scale; }

	void setScale(vec3 scale) { this->scale = scale; }

	void addScale(vec3 scale) { this->scale += scale; }

	vec3 getRotation() { return rotation; }

	void setRotation(vec3 rotation) { this->rotation = rotation; }

	void rotate(vec3 amount) { this->rotation += amount; }

	char* getObjectTypeTag() { return this->objectTypeTag; }

	//Constructors and Destructor

	Object()
	{
		this->isVisible = true;
	}

	Object(const char objectTypeTag[], vec3 position)
	{
		this->objectTypeTag = _strdup(objectTypeTag);
		this->position = position;
	}
};

struct Scene {

protected:
	std::vector<Object*> objList;
	char *name;
	EventHandler* eventSheet;
public:

	EventHandler* getEventHandler()
	{
		return this->eventSheet;
	}

	//Getters and setters

	char* getTitle() {
		return this->name;
	}

	void addObj(Object* obj)
	{
		this->objList.push_back(obj);
	}

	Object* objectAt(int index)
	{
		return this->objList.at(index);
	}

	//Constructors and Destructor

	Scene()
	{

	}

	Scene(const char name[])
	{
		this->name = _strdup(name);
	}

	~Scene()
	{
		//TODO Clear the parent of all it's childrens
	}

	//TODO Contructor for: "Scene(char name, Camera camera);"
};

