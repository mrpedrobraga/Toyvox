#pragma once

#include <stdio.h>
#include <math.h>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>

#include "components.h"

namespace tvx {

class VoxelModel
{
	//TODO Functions to import .3dsprite models
	//Transformation matrix

	//The models that can be accessed externally even after transform.
	//They will be used for PARENTING and other amazing stuff
	glm::vec3 modelPoints[];
};

typedef VoxelModel VoxelAnimFrame;

struct Scene {

	/*
	
								TODO (by Mr. Henri)

		Scenes will be the ones responsible by controlling the system of ECS.

		Scenes will also hold information about the... well... scene,
		like background color, Skybox Image, or Dynamic Skybox (sun and sky settings)

		Scenes will transform eventing into logic by messing with the components!

		You can add your own components to the enum that the scene looks for updating.
	*/
};
}
