
#pragma once

#include <stdio.h>
#include <math.h>
#include <vector>
#include <string.h>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>

#include "events.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
# define STRDUP _strdup
#else
# define STRDUP strdup
#endif

namespace tvx {

class VoxelModel
{
	//TODO Functions to import .tvx models
	//Transformation matrix
};

typedef VoxelModel VoxelAnimFrame;

class Object {
protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	//TODO use "mat4x4 LocRotScale;" instead of all those glm::vec3(s).

	bool isVisible;
	char *objectTypeTag;
public:
	//Getters, setters and adders.
	glm::vec3 getPosition() { return position; }

	void setPosition(glm::vec3 position) { this->position = position; }

	void move(glm::vec3 amount) { this->position += amount; }

	glm::vec3 getScale() { return scale; }

	void setScale(glm::vec3 scale) { this->scale = scale; }

	void addScale(glm::vec3 scale) { this->scale += scale; }

	glm::vec3 getRotation() { return rotation; }

	void setRotation(glm::vec3 rotation) { this->rotation = rotation; }

	void rotate(glm::vec3 amount) { this->rotation += amount; }

	char* getObjectTypeTag() { return this->objectTypeTag; }

	//Constructors and Destructor

	Object()
	{
		this->isVisible = true;
	}

	Object(const char *objectTypeTag, glm::vec3 position)
	{
		this->objectTypeTag = STRDUP(objectTypeTag);
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

	Scene(const char *name)
	{
		this->name = STRDUP(name);
	}

	~Scene()
	{
		//TODO Clear the parent of all it's childrens
	}

	//TODO Contructor for: "Scene(char name, Camera camera);"
};

}
