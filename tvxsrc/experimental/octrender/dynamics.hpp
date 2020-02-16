
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
				
				if (lClicked) {
					lClicked = false;
					glm::vec3 rayForward(0, 0, 1), camRot = player.getRot();
					rayForward = glm::rotateX(rayForward, -camRot.y);
					rayForward = glm::rotateY(rayForward, camRot.x);
					auto rayForwardResult = voctree.ray(player.getPos(), rayForward);
					if (rayForwardResult.hit) {
						// VoxelDword &target = voctree.at(rayForwardResult.pos + rayForwardResult.norm);
						VoxelDword &target = *rayForwardResult.vox;
						target.setRed(7);
						target.setGreen(0);
						target.setBlue(0);
						target.setIsFilled(true);
						voctree.updateGpu(0);
					}
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