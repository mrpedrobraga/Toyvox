
#pragma once

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
			void setChildren(uint8_t in);
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

	template<uint_fast64_t maxLvl>
	class Voxtree {
		public:
			static constexpr uint_fast64_t leafCount = sprout::pow(8, maxLvl); // FIXME: clang's sprout::pow is off-by-one?
			static constexpr uint_fast64_t scndCount = sprout::pow(8, maxLvl - 1);
			static constexpr uint_fast64_t valenceCount = leafCount + scndCount;
			static constexpr uint_fast64_t trunkCount = (leafCount - 1) / 7;
			static constexpr uint_fast64_t nuclearCount = (sprout::pow(8, maxLvl - 1) - 1) / 7;
			static constexpr uint_fast64_t totalCount = (sprout::pow(8, maxLvl + 1) - 1) / 7;

			explicit Voxtree() {
				buftex = std::make_unique<BufferTexture<totalCount * sizeof(VoxelDword)>>();
			}

			void insertLeaf(const VoxelDword &voxel, uint_fast64_t morton) {
				buftex->template writeToCpu<VoxelDword>(morton + trunkCount, voxel);
			}
			void insertLeaf(const VoxelDword &voxel, glm::uvec3 pos) {
				uint_fast32_t morton = libmorton::morton3D_32_encode(pos.x, pos.y, pos.z);
				insertLeaf(voxel, morton);
			}
			void updateGpu() {
				fill();
				buftex->sendToGpu();
				buftex->use(0);
			}

		private:

			struct Accumulator {
				VoxelDword voxel = {};
				uint_fast8_t red = 0, green = 0, blue = 0, metal = 0, rough = 0, light = 0, which = 0, count = 0;
				void add(const VoxelDword &v) {
					assert(which < 8);
					if ( ! v.getIsFilled()) {
						++which;
						return;
					}
					++count;
					voxel.setChildOn(static_cast<OctCoordCartesian>(which++));
					red += v.getRed();
					green += v.getGreen();
					blue += v.getBlue();
					metal += v.getIsMetal();
					rough += v.getRoughness();
					light += v.getLightness();
				}
				void add(VoxelDword &&v) { add(v); }
				bool isDirty() { return which; }
				VoxelDword avg() {
					if ( ! count) { return voxel; }
					voxel.setRed(red / count);
					voxel.setGreen(green / count);
					voxel.setBlue(blue / count);
					voxel.setNormal(26);
					voxel.setIsFilled(voxel.getChildMasked(0xFF));
					voxel.setIsMetal(metal / count);
					voxel.setRoughness(rough / count);
					voxel.setLightness(light / count);
					return voxel;
				}
				VoxelDword get() {
					return voxel;
				}
				void reset() {
					red = 0;
					green = 0;
					blue = 0;
					metal = 0;
					rough = 0;
					light = 0;
					which = 0;
					count = 0;
				}
			};
			
			std::unique_ptr<BufferTexture<totalCount * sizeof(VoxelDword)>> buftex;
			std::array<Accumulator, nuclearCount + scndCount> nuclearAccumulators;
			int whichScene = 0;
			
			VoxelDword getMortonColors(uint_fast64_t morton) {
				VoxelDword voxel;
				uint_fast16_t x, y, z;
				libmorton::morton3D_32_decode(morton, x, y, z);
				bool isFilled = ! (morton % 5);
				voxel.setIsFilled(isFilled);
				if (isFilled) {
					float cycler = 0.0043f * morton;
					voxel.setRed((7.f / 2.f) * (1 + sinf(cycler)));
					voxel.setGreen((7.f / 2.f) * (1 + sinf(cycler + M_PIf32 * (2.f / 3.f))));
					voxel.setBlue((7.f / 2.f) * (1 + sinf(cycler + M_PIf32 * (4.f / 3.f))));
					voxel.setRoughness(15);
					voxel.setLightness(0);
				}
				return voxel;
			}
			
			VoxelDword getAntisphere(uint_fast64_t morton) {
				VoxelDword voxel;
				uint_fast16_t x, y, z;
				libmorton::morton3D_32_decode(morton, x, y, z);
				bool isFilled = (x > 2 && x <= 13) && (y > 2 && y <= 13) && (z > 2 && z <= 13) &&
				                glm::length(glm::vec3(x, y, z) / 32.f - glm::vec3(0.25)) > 0.17f;
				voxel.setIsFilled(isFilled);
				if (isFilled) {
					voxel.setRed(x * 0.5f);
					voxel.setGreen(y * 0.5f);
					voxel.setBlue(z * 0.5f);
				}
				return voxel;
			}

			VoxelDword getDebugCorners(uint_fast64_t morton) {
				VoxelDword voxel;
				uint_fast16_t x, y, z;
				libmorton::morton3D_32_decode(morton, x, y, z);
				bool isFilled = (x == 0 || x == 15) && (y == 0 || y == 15) && (z == 0 || z == 15);
				voxel.setIsFilled(isFilled);
				if (isFilled) {
					voxel.setRed(x * 0.5f);
					voxel.setGreen(y * 0.5f);
					voxel.setBlue(z * 0.5f);
				}
				return voxel;
			}

			void fill() {
				uint_fast64_t valenceIdx = 0, leafIdx = 0, nuclearStart = valenceCount / 2, accumIdx = 0;
				int_fast64_t nuclearCrct = 0;
				for (; valenceIdx < valenceCount + nuclearCount; ++valenceIdx) {
					if (valenceIdx == nuclearStart) {
						nuclearCrct -= nuclearCount;
						valenceIdx += nuclearCount; // JUMP THE NUCLEUS
					}
					VoxelDword voxel;
					if ((valenceIdx + nuclearCrct + 1) % 9) {
						switch(whichScene) {
							case 0: voxel = getAntisphere(leafIdx++); break;
							case 1: voxel = getMortonColors(leafIdx++); break;
							case 2: voxel = getDebugCorners(leafIdx++); break;
						}
					}
					else {
						nuclearAccumulators[accumIdx].reset();
						for (int i = -8; i < 0; ++i) {
							nuclearAccumulators[accumIdx].add(*buftex->template cpu<VoxelDword>(valenceIdx + i));
						}
						voxel = nuclearAccumulators[accumIdx++].avg();
					}
					buftex->template writeToCpu<VoxelDword>(valenceIdx, voxel);
				}
				
				uint_fast64_t nuclearIdx = 0, nuclearInvLvl = 2, nuclearAvgHead = 0;
				for (; nuclearInvLvl <= maxLvl; ++nuclearInvLvl) {
					uint_fast32_t lvlLimit = pow(8, maxLvl - nuclearInvLvl);
					for (uint_fast32_t inLvl = 0; inLvl < lvlLimit; ++inLvl) {
						nuclearAccumulators[accumIdx].reset();
						uint_fast64_t nuclearAvgLimit = nuclearAvgHead + 8;
						for (; nuclearAvgHead < nuclearAvgLimit; ++nuclearAvgHead) {
							nuclearAccumulators[accumIdx].add(nuclearAccumulators[nuclearAvgHead].get());
						}
						buftex->template writeToCpu<VoxelDword>(nuclearStart + nuclearIdx++, nuclearAccumulators[accumIdx++].avg());
					}
				}
			}

	};
}