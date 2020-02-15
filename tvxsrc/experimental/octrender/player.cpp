
#include "player.hpp"

namespace tvx {
	void limitPosition(glm::vec4 &pos) {
		static constexpr float padLo = 0.0001f, padHi = 0.9999f;
		if (pos.x > padHi) { pos.x = padHi; }
		if (pos.x < padLo) { pos.x = padLo; }
		if (pos.y > padHi) { pos.y = padHi; }
		if (pos.y < padLo) { pos.y = padLo; }
		if (pos.z > padHi) { pos.z = padHi; }
		if (pos.z < padLo) { pos.z = padLo; }
	}
	void flyFree(glm::vec4 &pos, float amount, float pitch, float yaw) {
		pos.x += amount *  sinf(yaw) * cosf(pitch);
		pos.y += amount * -sinf(pitch);
		pos.z += amount *  cosf(yaw) * cosf(pitch);
		limitPosition(pos);
	}
	void walkFlat(glm::vec4 &walk, float amount, float pitch, float yaw) {
		walk.x += amount *  sinf(yaw) * cosf(pitch);
		walk.y += amount * -sinf(pitch);
		walk.z += amount *  cosf(yaw) * cosf(pitch);
	}
}