
#pragma once

#include "extern.hpp"

namespace tvx {
	
	class ScreenCoveringTriangle {
			GLuint vao = 0;
			GLuint vbo = 0;
		public:
			ScreenCoveringTriangle();
			[[nodiscard]] GLuint getVao() const;
	};
	
	template<uint64_t size, int bufType>
	class GpuBuffer {
			uint8_t cpuData[size] = {};
			GLuint handle = 0;
			GLuint blkIdx = 0;
		public:
			explicit GpuBuffer(GLuint idx) : blkIdx(idx) {
				glGenBuffers(1, &handle);
				glBindBuffer(bufType, handle);
				glBufferData(bufType, size, cpuData, GL_DYNAMIC_DRAW);
				glBindBufferBase(bufType, blkIdx, handle);
				glBindBuffer(bufType, 0);
			}
			template<typename T>
			[[nodiscard]] uint64_t getCapacity() const {
				return size / sizeof(T);
			}
			template<typename T>
			bool writeToCpu(uint64_t idx, uint64_t num, T const *data) {
				if ( idx < 0 || num <= 0 || ! data) { return false;}
				if (idx * sizeof(T) + num * sizeof(T) >= size) { return false; }
				memcpy(&reinterpret_cast<T *>(cpuData)[idx], data, num * sizeof(T));
				return true;
			}
			template<typename T>
			bool writeToCpu(uint64_t idx, const T &data) {
				return writeToCpu<T>(idx, 1, &data);
			}
			void sendToGpu() {
				glBindBuffer(bufType, handle);
				GLvoid *p = glMapBuffer(bufType, GL_WRITE_ONLY);
				memcpy(p, cpuData, size);
				glUnmapBuffer(bufType);
				glBindBuffer(bufType, 0);
			}
			static void reportUboSupport() {
				GLint maxSize, maxCount, maxComps, maxBlocks;
				glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxSize);
				glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxCount);
				glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxComps);
				glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxBlocks);
				SDL_Log("GPU supports %u UBO's of %u bytes each.\n", maxCount, maxSize);
				SDL_Log("Frag supports %u UBO's with a total of %u components (%u bytes)\n", maxBlocks, maxComps, maxComps * 4);
				if (maxComps * 4 < size) { SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failure to meet minimum requirements.\n"); }
			}
	};
}