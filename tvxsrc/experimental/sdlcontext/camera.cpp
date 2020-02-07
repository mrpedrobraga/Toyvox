
#include "camera.hpp"

namespace tvx {
	static void moveDependingOnFacingDirection(glm::vec4 &pos, float amount, float pitch, float yaw) {
		pos.x += amount *  sinf( yaw ) * cosf( pitch );
		pos.y += amount * -sinf( pitch );
		pos.z += amount *  cosf( yaw ) * cosf( pitch );
		static constexpr float padLo = 0.0001f, padHi = 0.9999f;
		if (pos.x > padHi) { pos.x = padHi; }
		if (pos.x < padLo) { pos.x = padLo; }
		if (pos.y > padHi) { pos.y = padHi; }
		if (pos.y < padLo) { pos.y = padLo; }
		if (pos.z > padHi) { pos.z = padHi; }
		if (pos.z < padLo) { pos.z = padLo; }
	}

	FreeCamera::FreeCamera(
				uint_fast64_t maxLvl, const glm::vec3 &start, const std::string &fw, const std::string &bw,
				const std::string &lf, const std::string &rt, const std::string &up, const std::string &dn)
				: rot(), pos(glm::vec4(start, 1.f)), ctrl(1, 1, 0, maxLvl), curLvl(maxLvl), maxLvl(maxLvl),
				  mouseMoveSub("mouse_moved", [&](void *data) -> void {
					  auto e = reinterpret_cast<SDL_Event *>(data);
					  yaw += e->motion.xrel * dt * sensitivity * aspect;
					  pitch += e->motion.yrel * dt * sensitivity;
					  if (yaw > M_PI * 2.0) { yaw -= M_PI * 2.0; }
					  if (yaw < 0.0) { yaw += M_PI * 2.0; }
					  if (pitch > M_PI_2) { pitch = M_PI_2; }
					  if (pitch < -M_PI_2) { pitch = -M_PI_2; }
					  rot.x = yaw;
					  rot.y = -pitch;
				  }),
				  mouseLeftSub("mouse_held_left", [&](void *data) -> void {
				  	rot.z = -1;
				  }),
				  mouseRightSub("mouse_held_right", [&](void *data) -> void {
					  rot.w = -1;
				  }),
				  upDetSub("mouse_down_x1", [&](void *data) -> void {
					  ctrl.x = -1;
					  if (--curLvl < 1) { curLvl += this->maxLvl; }
					  ctrl.w = curLvl;
				  }),
				  dnDetSub("mouse_down_x2", [&](void *data) -> void {
					  ctrl.y = -1;
					  if (++curLvl > this->maxLvl) { curLvl -= this->maxLvl; }
					  ctrl.w = curLvl;
				  }),
				  gridSub("mouse_wheel", [&](void *data) -> void {
				  	auto val = reinterpret_cast<int*>(data);
				  	gridMode += *val;
				  	if (gridMode > 2) { gridMode -= 3; }
				  	if (gridMode < 0) { gridMode += 3; }
				  	ctrl.z = gridMode;
				  }),
				  fwSub("key_held_" + fw, [&](void *data) -> void {
					  moveDependingOnFacingDirection(pos, static_cast<float>(dt) * static_cast<float>(speed),
					                                 static_cast<float>(pitch), static_cast<float>(yaw));
				  }),
				  bwSub("key_held_" + bw, [&](void *data) -> void {
					  moveDependingOnFacingDirection(pos, static_cast<float>(dt) * static_cast<float>(speed),
					                                 static_cast<float>(pitch + M_PI), static_cast<float>(yaw));
				  }),
				  lfSub("key_held_" + lf, [&](void *data) -> void {
					  moveDependingOnFacingDirection(pos, static_cast<float>(dt) * static_cast<float>(speed),
					                                 static_cast<float>(0), static_cast<float>(yaw - M_PI_2));
				  }),
				  rtSub("key_held_" + rt, [&](void *data) -> void {
					  moveDependingOnFacingDirection(pos, static_cast<float>(dt) * static_cast<float>(speed),
					                                 static_cast<float>(0), static_cast<float>(yaw + M_PI_2));
				  }),
				  upSub("key_held_" + up, [&](void *data) -> void {
					  moveDependingOnFacingDirection(pos, static_cast<float>(dt) * static_cast<float>(speed),
					                                 static_cast<float>(pitch - M_PI_2), static_cast<float>(yaw));
				  }),
				  dnSub("key_held_" + dn, [&](void *data) -> void {
					  moveDependingOnFacingDirection(pos, static_cast<float>(dt) * static_cast<float>(speed),
					                                 static_cast<float>(pitch + M_PI_2), static_cast<float>(yaw));
				  }) { }
	glm::vec4 FreeCamera::getRot() {
		return rot;
	}
	glm::vec4 FreeCamera::getPos() {
		return pos;
	}
	glm::vec4 FreeCamera::getCtrl() {
		return ctrl;
	}
	void FreeCamera::refresh(float delta) {
		dt = delta;
		rot.z = 1;
		rot.w = 1;
		ctrl.x = 1;
		ctrl.y = 1;
	}
	void FreeCamera::setAspect(float wOverH) {
		aspect = wOverH;
	}
}