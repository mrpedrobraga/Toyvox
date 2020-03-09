#include "options.h"
#include "components.h"
#include "game.h"
#include "voxel_objects.h"

/* Usage:

To add any of those components to a scene, use:

<ComponentType> <name> = <ComponentType>();

To add a system, add it to a scene's tick function, or to a global tick function!

 */

using namespace tvx;

//Transform
using Transform = glm::mat4x4;
typedef ComponentSet<Transform> Transforms;

//Type Tag (to easily differenciate objects!)
typedef ComponentSet<TypeTag> TypeTags;

//Kinematics (velocity and a lot of "moving objects" util)
struct c_Kinematics {
  glm::vec3 velocity;
  bool active = true;
};

typedef ComponentSet<c_Kinematics> Kinematics;

//Kinematics tick function. Will update objects that have both a c_Kinematics and a Transform component.
void tick_Kinematics(EntityUID* picked_entities, size_t picked_count, Transforms t, Kinematics k)
{
  for(size_t i = 0; i < picked_count; i++)
  {
      EntityUID cE = picked_entities[i];
      if(k[cE].active) t[cE] *= glm::translate(k[cE].velocity * tvx::game::DELTA_TIME);
  }
}
