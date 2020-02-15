
#pragma once

#include "player.hpp"

namespace tvx {
	template<uint_fast64_t maxVoxLvl>
	class Dynamics {
		public:
			
			void tick(float dt, Player<maxVoxLvl> &player, Voctree<maxVoxLvl> &voctree) {
				
				if (player.freeMove) { return; }
				
				glm::vec3 rayForward(0, 0, 1), camRot = player.getRot();
				rayForward.zy() = glm::mat2(cos(camRot.y), -sin(camRot.y), sin(camRot.y), cos(camRot.y)) * rayForward.zy();
				rayForward.zx() = glm::mat2(cos(camRot.x), -sin(camRot.x), sin(camRot.x), cos(camRot.x)) * rayForward.zx();
				auto rayForwardResult = voctree.ray(player.getPos(), rayForward);
				
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