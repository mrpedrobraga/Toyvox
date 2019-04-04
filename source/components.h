
#pragma once

#include <stdio.h>
#include <string.h>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>

namespace tvx {

	/*
		Here, there's a list of default components you may want to add to your objects!
	*/

	struct TypeTag {

		//TODO: Change to a better way of making strings
		char tag[];
	};

	struct Position {
		
		glm::vec3 value;
	};

	struct Scale {

		glm::vec3 scale;
	};

	struct Rotation {

		glm::vec3 rotation;
	};

	struct Velocity {

		glm::vec3 value;
	};

	struct AngularVelocity {

		glm::vec3 value;
	};

	struct Gravity {

		glm::vec3 value;
	};

	struct PointOfView {

		float hFOV = 90;
		float vFOV = 90 * 16 / 9;
		//By default cameras have an 16:9 aspect ratio!
	};

	struct SolidModels {
		
		//TODO Add Groups of VoxelModels here.
		//It differs from ParticleSystem, because every model can have its own imagepoints
		//and animation keyframes

	};

	struct ParticleSystem {

		//TODO Add Groups of VoxelModels here.
		//It differs from SolidModels, because every model can have its own physics. 

	};

	struct Emitter {

		double emmision;
	};

	//--------------------------------------------------------------------------//
	//This is the default enum component. Be careful when editing it!

	enum Collection {

		COMPONENT_NONE				= 0,
		COMPONENT_TYPETAG			= 1,
		COMPONENT_POSITION			= 1 << 1,
		COMPONENT_SCALE				= 1 << 2,
		COMPONENT_ROTATION			= 1 << 3,
		COMPONENT_VELOCITY			= 1 << 4,
		COMPONENT_ANGULARVELOCITY	= 1 << 5,
		COMPONENT_GRAVITY			= 1 << 6,
		COMPONENT_POV				= 1 << 7,
		COMPONENT_SOLIDMODELS		= 1 << 8,
		COMPONENT_PARTICLESYSTEM	= 1 << 9,
		COMPONENT_EMITTER			= 1 << 10

	};
}
