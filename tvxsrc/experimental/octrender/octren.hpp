
#pragma once

#include <deque>
#include "sprout/math/pow.hpp"
#include "extern.hpp"
#include "morton.h"
#include "Octree.hpp"
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

	template<uint_fast64_t maxLvl, uint_fast64_t maxLeafBytes = 65536> // 65536 is spec-required UBO minimum
	class Voxtree {
		public:
			static constexpr uint_fast64_t bufType = GL_UNIFORM_BUFFER;
			static constexpr uint_fast64_t leafCount = sprout::pow(8, maxLvl); // FIXME: clang's sprout::pow is off-by-one?
			static constexpr uint_fast64_t trunkCount = (leafCount - 1) / 7;
			static constexpr uint_fast64_t maxTrunkBytes = trunkCount * sizeof(VoxelDword);

			explicit Voxtree(GLuint voxBind, GLuint nodeBind) : leaves(voxBind), octree(pow(2, maxLvl)) {
				GeneralBuffer<maxLeafBytes, bufType>::reportUboSupport();
				textree = std::make_unique<DataTexture
							<2048 * 2048 * 4, 2048, 2048, GL_BGRA, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, GL_NEAREST>>();
			}

			void insertLeaf(const VoxelDword &voxel, uint_fast64_t morton) {
				leaves.template writeToCpu<VoxelDword>(morton + trunkCount, voxel);
			}
			void insertLeaf(const VoxelDword &voxel, glm::uvec3 pos) {
				uint_fast32_t morton = libmorton::morton3D_32_encode(pos.x, pos.y, pos.z);
				insertLeaf(voxel, morton);
			}
			void updateGpu() {
				fillTestCorners();
				leaves.sendToGpu();
				// recurseLod();
				textree->sendToGpu();
				textree->use(0);
			}

		private:
			static_assert(leafCount * sizeof(VoxelDword) < maxLeafBytes, "Max tree depth too large.");
			GeneralBuffer<maxLeafBytes, bufType> leaves;
			std::unique_ptr<DataTexture
			      <2048 * 2048 * 4, 2048, 2048, GL_BGRA, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, GL_NEAREST>> textree;
			Octree<VoxelDword> octree;
			
			void fillLeavesMortonColorsRandomized() {
				for (uint_fast64_t i = 0; i < leafCount; ++i) {
					VoxelDword voxel;
					voxel.setIsFilled(! (i % 5));
					float cycler = 0.0043f * i;
					voxel.setRed((7.f / 2.f) * (1 + sinf(cycler)));
					voxel.setGreen((7.f / 2.f) * (1 + sinf(cycler + M_PIf32 * (2.f / 3.f))));
					voxel.setBlue((7.f / 2.f) * (1 + sinf(cycler + M_PIf32 * (4.f / 3.f))));
					
					voxel.setRoughness(15);
					voxel.setLightness(0);
					
					// insertLeaf(voxel, i);
					uint_fast16_t x, y, z;
					libmorton::morton3D_32_decode(i, x, y, z);
					octree.insert(x, y, z, voxel);
					textree->writeToCpu<VoxelDword>(i, voxel);
				}
				for (uint_fast64_t i = leafCount; i < leaves.template getCapacity<VoxelDword>(); ++i) {
					VoxelDword voxel;
					voxel.setIsFilled(false);
					insertLeaf(voxel, i);
				}
			}

			void fillLeavesAntisphere() {
				for (uint_fast64_t i = 0; i < leafCount; ++i) {
					VoxelDword voxel;
					uint_fast16_t x, y, z;
					libmorton::morton3D_32_decode(i, x, y, z);
					glm::vec3 pos(x, y, z);
					voxel.setIsFilled((x > 2 && x <= 13) && (y > 2 && y <= 13) && (z > 2 && z <= 13) &&
					                  glm::length(pos / 32.f - glm::vec3(0.25)) > 0.2f);
					voxel.setRed(x / 2.f);
					voxel.setGreen(y / 2.f);
					voxel.setBlue(z / 2.f);
					insertLeaf(voxel, i);
					octree.insert(x, y, z, voxel);
					textree->writeToCpu<VoxelDword>(trunkCount + i, voxel);
				}
				for (uint_fast64_t i = leafCount; i < leaves.template getCapacity<VoxelDword>(); ++i) {
					VoxelDword voxel;
					voxel.setIsFilled(false);
					insertLeaf(voxel, i);
				}
			}

			void fillTestCorners() {
				for (uint_fast64_t i = 0; i < leafCount; ++i) {
					VoxelDword voxel;
					uint_fast16_t x, y, z;
					libmorton::morton3D_32_decode(i, x, y, z);
					voxel.setIsFilled((x == 0 || x == 15) && (y == 0 || y == 15) && (z == 0 || z == 15));
					voxel.setRed(x / 2.f);
					voxel.setGreen(y / 2.f);
					voxel.setBlue(z / 2.f);
					insertLeaf(voxel, i);
					octree.insert(x, y, z, voxel);
					textree->writeToCpu<VoxelDword>(i, voxel);
				}
				for (uint_fast64_t i = leafCount; i < leaves.template getCapacity<VoxelDword>(); ++i) {
					VoxelDword voxel;
					voxel.setIsFilled(false);
					insertLeaf(voxel, i);
				}
			}
			
			struct Accumulator {
				uint_fast8_t red = 0, green = 0, blue = 0, metal = 0, rough = 0, light = 0, which = 0;
				VoxelDword voxel = {};
				void add(const VoxelDword &v) {
					if ( ! v.getIsFilled()) {
						++which;
						return;
					}
					voxel.setChildOn(static_cast<OctCoordCartesian>(which++));
					red += v.getRed();
					green += v.getGreen();
					blue += v.getBlue();
					metal += v.getIsMetal();
					rough += v.getRoughness();
					light += v.getLightness();
				}
				void add(VoxelDword &&v) { add(v); }
				VoxelDword avg() {
					voxel.setRed(red / 8);
					voxel.setGreen(green / 8);
					voxel.setBlue(blue / 8);
					voxel.setNormal(26);
					voxel.setIsFilled(voxel.getChildMasked(0xFF) == 0xFF);
					voxel.setIsMetal(metal / 8);
					voxel.setRoughness(rough / 8);
					voxel.setLightness(light / 8);
					return voxel;
				}
			};
			void recurseLod() {
				uint_fast64_t head = 0, level = 0;
				recurseLod(octree.root(), head, level);
			}
			VoxelDword recurseLod(Octree<VoxelDword>::Node *node, uint_fast64_t &head, uint_fast64_t level) {
				std::unique_ptr<Accumulator> accum;
				if (level < maxLvl) {
					++level;
					auto branch = reinterpret_cast<Octree<VoxelDword>::Branch *>(node);
					accum = std::make_unique<Accumulator>();
					accum->add(recurseLod((*branch)[0], head, level));
					accum->add(recurseLod((*branch)[1], head, level));
					accum->add(recurseLod((*branch)[2], head, level));
					accum->add(recurseLod((*branch)[3], head, level));
					// publishf("log", "Level %llu writing to %10llu", level - 1, head);
					uint_fast64_t myHead = head++;
					accum->add(recurseLod((*branch)[4], head, level));
					accum->add(recurseLod((*branch)[5], head, level));
					accum->add(recurseLod((*branch)[6], head, level));
					accum->add(recurseLod((*branch)[7], head, level));
					VoxelDword v = accum->avg();
					*textree->cpu<VoxelDword>(myHead) = v;
					return v;
				} else {
					auto leaf = reinterpret_cast<Octree<VoxelDword>::Leaf*>(node);
					return leaf->value();
				}
			}
	};
}