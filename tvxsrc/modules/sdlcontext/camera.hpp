
#pragma once

#include "extern.hpp"
#include "topics.hpp"

namespace tvx {
	class FreeCamera {
			glm::vec3 pos, rot;
			double pitch = 0.0, yaw = 0.0, dt = 0.0;
			Subscription mouseMoveSub, mouseClickSub, fwSub, bwSub, lfSub, rtSub, upSub, dnSub;
		public:
			explicit FreeCamera(
						const glm::vec3 &start = glm::vec3(),
						const std::string &fw = "w",
						const std::string &bw = "s",
						const std::string &lf = "a",
						const std::string &rt = "d",
						const std::string &up = "space",
						const std::string &dn = "c"
			);
			glm::vec3 getRot();
			glm::vec3 getPos();
			void setDelta(float delta);
	};
}