#pragma once

#include <stdio.h>
#include <math.h>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>

#include "components.h"

namespace tvx {

static const int ENTITY_COUNT = 100;
//The amount of test entities. This value is temporary, because in the final it's dynamic allocated.

struct Scene {

	/*
								TODO (by Mr. Henri)

		Scenes will be the ones responsible by controlling the entities (as a group) of the ECS.

		Scenes will also hold information about the... well... scene,
		like background color, Skybox Image, or Dynamic Skybox (sun and sky settings)

		Scenes will group entities so they can be affected by the system (the eventHandler)

		You can add your own components by creating more arrays.
	
								(! IMPORTANT !)
			Note that right now I'm using a fixed normal array!
			But i'm going to switch it to a dynamic array after we have that custom
			memory allocation!
	
		The `mask` array stores Entities(ints) that contain data about 
		which components it contains. In binary.

		The component arrays contain the corresponding components for each
		entity (index).

	*/

	int mask[ENTITY_COUNT];
	Position position[ENTITY_COUNT];
	TypeTag objectTypeTag[ENTITY_COUNT];
	Velocity velocity[ENTITY_COUNT];

};

//Retrieves the first null space, for us to override with a custom mask
//It's just like we do in actual memory ^^
unsigned int createEntity(Scene *scene)
{
	unsigned int entity;
	for (entity = 0; entity < ENTITY_COUNT; ++entity)
	{
		if (scene->mask[entity] == COMPONENT_NONE)
		{
			return(entity);
		}
	}

	printf("So yeah, you reached the limit, bro.\n");
	return(ENTITY_COUNT);
}

//This is a test function that creates a test object. May (or may not) be removed later.
unsigned int createTestObject(Scene *scene, glm::vec3 position)
{
	unsigned int entity = createEntity(scene);

	scene->mask[entity] = COMPONENT_POSITION | COMPONENT_TYPETAG;

	scene->position[entity].value = position;
	scene->objectTypeTag[entity].tag = "testObject";

	return(entity);
}

//Sets a certain space to null, so no events can target that entity.
//It basically dies.
void destroyEntity(Scene *scene, unsigned int entity)
{
	scene->mask[entity] = COMPONENT_NONE;
}
}
