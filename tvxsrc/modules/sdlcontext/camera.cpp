
#include "camera.hpp"

namespace tvx {

	FreeCamera::FreeCamera(
				const glm::vec3 &start, const std::string &fw, const std::string &bw,
				const std::string &lf, const std::string &rt, const std::string &up, const std::string &dn)
				: rot(), pos(start),
				  mouseMoveSub("mouse_moved", [&](void *data) -> void {
					  auto e = reinterpret_cast<SDL_Event *>(data);
					  yaw += e->motion.xrel * dt;
					  pitch += e->motion.yrel * dt;
					  rot.x = yaw;
					  rot.y = -pitch;
				  }),
				  mouseClickSub("mouse_down_left", [&](void *data) -> void {
				  	rot.z = -1;
				  }),
				  fwSub("key_held_" + fw, [&](void *data) -> void {
					  pos += glm::vec3(0, 0, .01);
				  }),
				  bwSub("key_held_" + bw, [&](void *data) -> void {
					  pos += glm::vec3(0, 0, -.01);
				  }),
				  lfSub("key_held_" + lf, [&](void *data) -> void {
					  pos += glm::vec3(-.01, 0, 0);
				  }),
				  rtSub("key_held_" + rt, [&](void *data) -> void {
					  pos += glm::vec3(.01, 0, 0);
				  }),
				  upSub("key_held_" + up, [&](void *data) -> void {
					  pos += glm::vec3(0, .01, 0);
				  }),
				  dnSub("key_held_" + dn, [&](void *data) -> void {
					  pos += glm::vec3(0, -.01, 0);
				  }) { }
	glm::vec3 FreeCamera::getRot() {
		return rot;
	}
	glm::vec3 FreeCamera::getPos() {
		return pos;
	}
	void FreeCamera::setDelta(float delta) {
		dt = delta;
	}
}