#include "components.h"
#include "game.h"
#include "voxel_objects.h"

/* Usage:

To add any of those components to a scene, use:

<ComponentType> <name> = <ComponentType>();

To add a system, add it to a scene's tick function, or to a global tick function!

 */

using namespace tvx;

//Position 2d
typedef ComponentSet<glm::vec2> Positions2D;

//Position 2d
typedef ComponentSet<glm::vec3> Positions3D;

//Transform
typedef ComponentSet<glm::mat4x4> Transforms;

//Type Tag (to easily differenciate objects!)
typedef ComponentSet<std::string> TypeTags;

/*3D movement
struct Kinematics3D {
	glm::mat4x4 transform;
	glm::vec3 velocity;
	glm::vec3 acceleration;
};

inline void Kinematics3D_tick(Kinematics3D& k) {
	k.velocity += acceleration * DELTA_TIME;
	transform = glm::translate(transform, velocity * DELTA_TIME);
}

typedef ComponentSet<Kinematics3D> Kinematics3D;

//2D movement
struct Kinematics2D {
	glm::mat3x3 transform;
	glm::vec2 velocity;
	glm::vec2 acceleration;
};

inline void Kinematics2D_tick(Kinematics2D& k) {
	k.velocity += acceleration * DELTA_TIME;
	transform = glm::translate(transform, velocity * DELTA_TIME);
}

typedef ComponentSet<Kinematics2D> Kinematics2D;

//Voxel Models!
typedef ComponentSet<VoxelObject> VoxelObjects;*/