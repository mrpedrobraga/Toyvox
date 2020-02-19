
#pragma once

#include "player.hpp"

namespace tvx {
	template<uint_fast64_t maxVoxLvl>
	class Dynamics {
			Subscription lClickSub, rClickSub;
			bool lClicked = false, rClicked = false;

		public:

			Dynamics()
						:
						lClickSub("mouse_down_left", [&](void *data) -> void { lClicked = true; }),
						rClickSub("mouse_down_right", [&](void *data) -> void { rClicked = true; })
			{ }
			
			void tick(float dt, Player<maxVoxLvl> &player, Voctree<maxVoxLvl> &voctree) {
				
				if (lClicked || rClicked) {
					glm::vec3 rayForward(0, 0, 1), camRot = player.getRot();
					rayForward = glm::rotateX(rayForward, -camRot.y);
					rayForward = glm::rotateY(rayForward, camRot.x);
					auto rayFRes = voctree.ray(player.getPos(), rayForward);
					if (rayFRes.hit) {
						uint_fast64_t accIdx = UINT64_MAX;
						if (lClicked) {
							glm::vec3 cart = rayFRes.pos + rayFRes.norm;
							accIdx = libmorton::morton3D_32_encode(cart.x, cart.y, cart.z) / 8;
							VoxelDword &target = voctree.at(cart);
							target.setRed(7);
							target.setGreen(0);
							target.setBlue(0);
							target.setIsFilled(true);
						} else if (rClicked) {
							accIdx = libmorton::morton3D_32_encode(rayFRes.pos.x, rayFRes.pos.y, rayFRes.pos.z) / 8;
							VoxelDword &target = *rayFRes.vox;
							target.setIsFilled(false);
						}
						voctree.cascadeChanges(accIdx);
						voctree.sendToGpu(0);
					}
					lClicked = false;
					rClicked = false;
				}
				
				if (player.freeMove) { return; }
				
				glm::vec3 rayDownward(0, -1, 0);
				auto rayDownwardResult = voctree.ray(player.getPos(), rayDownward);
				
				if (rayDownwardResult.dist > Player<maxVoxLvl>::playerEyeLevel) {
					player.setIsGrounded(false);
					player.accelerate({0, -9.81 * Voctree<maxVoxLvl>::toMeters, 0});
				} else {
					player.setIsGrounded(true);
					if (player.stopFall()) { player.adjustHeight(Player<maxVoxLvl>::playerEyeLevel - rayDownwardResult.dist); }
				}
				player.tickDynamics();
			}
		private:
	};
	
}