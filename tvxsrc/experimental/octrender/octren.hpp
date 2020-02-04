
#pragma once

#include "sprout/math/pow.hpp"
#include "extern.hpp"
#include "morton.h"
#include "buffers.hpp"
#include "topics.hpp"

namespace tvx {
	enum class OctCoordCartesian {
			XNYNZN = 0,
			XNYNZP,
			XNYPZN,
			XNYPZP,
			XPYNZN,
			XPYNZP,
			XPYPZN,
			XPYPZP,
	};
	
	class VoxelDword {
			uint8_t data[4] = {};
		public:
			
			static uint64_t mortonize(glm::uvec3 pos);
			static glm::uvec3 demortonize(uint64_t m);
			
			[[nodiscard]] uint32_t getBits();
			[[nodiscard]] uint_fast8_t getChildMasked(uint_fast8_t mask) const;
			[[nodiscard]] bool getIsFilled() const;
			[[nodiscard]] bool getIsMetal() const;
			[[nodiscard]] uint_fast8_t getRed() const;
			[[nodiscard]] uint_fast8_t getGreen() const;
			[[nodiscard]] uint_fast8_t getBlue() const;
			[[nodiscard]] uint_fast8_t getNormal() const;
			[[nodiscard]] uint_fast8_t getRoughness() const;
			[[nodiscard]] uint_fast8_t getLightness() const;
			void setBits(uint32_t in);
			void setChildOn(OctCoordCartesian which);
			void setIsFilled(bool in);
			void setIsMetal(bool in);
			void setRed(uint_fast8_t in);
			void setGreen(uint_fast8_t in);
			void setBlue(uint_fast8_t in);
			void setNormal(uint_fast8_t in);
			void setRoughness(uint_fast8_t in);
			void setLightness(uint_fast8_t in);
			
			/*
			 * Lightness goes from darkened (occluded) to acting as a light emitter. Also the extreme darkest value codes for
			 * "true black" which reflects no light, while the extreme lightest value codes for "sky."
			 */
	};

	template<uint_fast64_t maxLvl = 4, uint_fast64_t maxBytes = 65536> // 65536 is spec-required UBO minimum
	class Voxtree {
			static constexpr uint_fast64_t bufType = GL_UNIFORM_BUFFER;
			static_assert(sprout::pow(8, maxLvl) * sizeof(VoxelDword) < maxBytes, "Max tree depth exceeds memory allotment.");
			GeneralBuffer<maxBytes, bufType> voxels;
		public:
			explicit Voxtree(GLuint binding) : voxels(binding) { GeneralBuffer<maxBytes, bufType>::reportUboSupport(); }
			void insertLeaf(const VoxelDword &voxel, uint_fast64_t morton) {
				voxels.template writeToCpu<VoxelDword>(morton + getTrunkSize(), voxel);
			}
			void insertLeaf(const VoxelDword &voxel, glm::uvec3 pos) {
				uint_fast32_t morton = libmorton::morton3D_32_encode(pos.x, pos.y, pos.z);
				insertLeaf(voxel, morton);
			}
			void updateGpu() {
				uint_fast64_t visibleLimit = pow(pow(2, maxLvl), 3);
				for (uint_fast64_t i = 0; i < visibleLimit; ++i) {
					VoxelDword voxel;
					voxel.setIsFilled(! (i % 5));
					float cycler = 0.0043f * i;
					voxel.setRed((7.f / 2.f) * (1 + sinf(cycler)));
					voxel.setGreen((7.f / 2.f) * (1 + sinf(cycler + M_PIf32 * (2.f / 3.f))));
					voxel.setBlue((7.f / 2.f) * (1 + sinf(cycler + M_PIf32 * (4.f / 3.f))));
					insertLeaf(voxel, i);
				}
				for (uint_fast64_t i = visibleLimit; i < voxels.template getCapacity<VoxelDword>(); ++i) {
					VoxelDword voxel;
					voxel.setIsFilled(false);
					insertLeaf(voxel, i);
				}
				voxels.sendToGpu();
			}
			[[nodiscard]] inline uint_fast32_t getTrunkSize() const { return (pow(8, maxLvl) - 1) / 7; }
	};
}