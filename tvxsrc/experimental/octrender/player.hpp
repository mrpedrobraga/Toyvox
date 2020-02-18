
#pragma once

#include "voctree.hpp"

namespace tvx {
	
	void limitPosition(glm::vec4 &pos);
	void flyFree(glm::vec4 &pos, float amount, float pitch, float yaw);
	void walkFlat(glm::vec4 &pos, float amount, float pitch, float yaw);
	
	/**
	 * Quick-and-dirty user control mechanism for testing
	 */
	template<int_fast32_t maxLvl>
	class Player {
			glm::vec4 pos, vel, rot, ctrl, walk;
			double pitch = 0.001337, yaw = 0.001337;
			float dt = 0.f, gridMode = 0.f, aspect = 1.f, sensitivity = 0.5, speed = 8.0 * Voctree<maxLvl>::toMeters;
			int_fast32_t curLvl;
			bool jumpInProgress = false, isGrounded = false;
			Subscription mouseMoveSub, mouseLeftSub, mouseRightSub, upDetSub, dnDetSub, gridSub, jumpSub, flySub;
			Subscription fwSub, bwSub, lfSub, rtSub, upSub, dnSub;
			static constexpr uint_fast32_t numModes = 4;

			enum class Movement { UP = 1, DOWN, FORWARD, BACKWARD, LEFT, RIGHT };
			void move(Movement which) {
				switch(which) {
					case Movement::UP: {
						if (freeMove) { flyFree(pos, dt * speed * 8, static_cast<float>(pitch - M_PI_2), static_cast<float>(yaw)); }
					} break;
					case Movement::DOWN: {
						if (freeMove) { flyFree(pos, dt * speed * 8, static_cast<float>(pitch + M_PI_2), static_cast<float>(yaw)); }
					} break;
					case Movement::FORWARD: {
						if (freeMove) { flyFree(pos, dt * speed * 8, static_cast<float>(pitch), static_cast<float>(yaw)); }
						else { walkFlat(walk, 1, 0.f, static_cast<float>(yaw)); }
					} break;
					case Movement::BACKWARD: {
						if (freeMove) { flyFree(pos, dt * speed * 8, static_cast<float>(pitch + M_PI), static_cast<float>(yaw)); }
						else { walkFlat(walk, 1, M_PI, static_cast<float>(yaw)); }
					} break;
					case Movement::LEFT: {
						if (freeMove) { flyFree(pos, dt * speed * 8, 0.f, static_cast<float>(yaw - M_PI_2)); }
						else { walkFlat(walk, 1, 0, static_cast<float>(yaw - M_PI_2)); }
					} break;
					case Movement::RIGHT: {
						if (freeMove) { flyFree(pos, dt * speed * 8, 0.f, static_cast<float>(yaw + M_PI_2)); }
						else { walkFlat(walk, 1, 0, static_cast<float>(yaw + M_PI_2)); }
					} break;
				}
			}
			
		public:
			static constexpr float playerEyeLevel = 1.75f * Voctree<maxLvl>::toMeters;
			bool freeMove = false;
			
			explicit Player(
						const glm::vec3 &start = glm::vec3(),
						const std::string &fw = "w",
						const std::string &bw = "s",
						const std::string &lf = "a",
						const std::string &rt = "d",
						const std::string &up = "space",
						const std::string &dn = "c",
						const std::string &jmp = "space",
						const std::string &fly = "tab"
			) : pos(glm::vec4(start, 1.f)), vel(0), rot(), ctrl(1, 1, 0, maxLvl), walk(0.f), curLvl(maxLvl),
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
			   mouseLeftSub("mouse_held_left", [&](void *data) -> void { rot.z = -1; }),
			   mouseRightSub("mouse_held_right", [&](void *data) -> void { rot.w = -1; }),
			   upDetSub("mouse_down_x1", [&](void *data) -> void {
				   if (--curLvl < 0) { curLvl += maxLvl + 1; }
				   ctrl.w = curLvl;
			   }),
			   dnDetSub("mouse_down_x2", [&](void *data) -> void {
				   if (++curLvl > maxLvl) { curLvl -= (maxLvl + 1); }
				   ctrl.w = curLvl;
			   }),
			   gridSub("mouse_wheel", [&](void *data) -> void {
				   auto val = reinterpret_cast<int*>(data);
				   gridMode += static_cast<float>(*val);
				   if (gridMode > numModes - 1) { gridMode -= numModes; }
				   if (gridMode < 0) { gridMode += numModes; }
				   ctrl.z = gridMode;
			   }),
			   fwSub("key_held_" + fw, [&](void *data) -> void { move(Movement::FORWARD); }),
			   bwSub("key_held_" + bw, [&](void *data) -> void { move(Movement::BACKWARD); }),
			   lfSub("key_held_" + lf, [&](void *data) -> void { move(Movement::LEFT); }),
			   rtSub("key_held_" + rt, [&](void *data) -> void { move(Movement::RIGHT); }),
			   upSub("key_held_" + up, [&](void *data) -> void { move(Movement::UP); }),
			   dnSub("key_held_" + dn, [&](void *data) -> void { move(Movement::DOWN); }),
			   jumpSub("key_down_" + jmp, [&](void *data) -> void { jump(); }),
			   flySub("key_down_" + fly, [&](void *data) -> void { freeMove = !freeMove; })
			   { }
			[[nodiscard]] glm::vec4 getPos() const {
				glm::vec4 ret = pos;
				ret.w = maxLvl;
				return ret;
			}
			[[nodiscard]] glm::vec4 getVel() const { return vel; }
			[[nodiscard]] glm::vec4 getRot() const { return rot; }
			[[nodiscard]] glm::vec4 getCtrl() const { return ctrl; }
			[[nodiscard]] glm::vec4 getWalk() const { return walk; }
			void refresh(float delta) {
				dt = delta;
				rot.z = 1;
				rot.w = 1;
			}
			void setAspect(float wOverH) { aspect = wOverH; }
			void accelerate(const glm::vec3 &dir) { vel += glm::vec4(dir * dt, 0); }
			void applyImpulse(const glm::vec3 &imp) { vel += glm::vec4(imp, 0); }
			void adjustHeight(float adj) { pos.y += adj; }
			bool stopFall() {
				if (vel.y <= 0.f) {
					vel.y = 0.f;
					return true;
				}
				return false;
			}
			void jump() {
				if (freeMove) { return; }
				if ( ! jumpInProgress && isGrounded) {
					jumpInProgress = true;
					applyImpulse({0, playerEyeLevel * 3.f, 0});
				}
			}
			void setIsGrounded(bool is = true) {
				if (is && vel.y <= 0.f) {
					isGrounded = is;
					jumpInProgress = false;
				}
			}
			void tickDynamics() {
				if (freeMove) {
					vel = glm::vec4(0);
				} else {
					if (glm::length(walk)) {
						walk = glm::normalize(walk);
						pos += walk * speed * dt;
					}
					pos += vel * dt;
					limitPosition(pos);
				}
				walk = glm::vec4(0);
			}
	};
}