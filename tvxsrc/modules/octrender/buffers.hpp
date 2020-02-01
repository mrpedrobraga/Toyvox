
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
	
	template<int format, int type, int filter, bool depth = false>
	class IntermediateTexture {
			GLuint fb = 0, db = 0, tex = 0;
			uint_fast64_t width, height;
		public:
			IntermediateTexture(uint_fast64_t w, uint_fast64_t h) : width(w), height(h) {
				glGenFramebuffers(1, &fb);
				glBindFramebuffer(GL_FRAMEBUFFER, fb);

				glGenTextures(1, &tex);
				glBindTexture(GL_TEXTURE_2D, tex);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
				
				if (depth) {
					glGenRenderbuffers(1, &db);
					glBindRenderbuffer(GL_RENDERBUFFER, db);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, db);
				}

				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
				GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, DrawBuffers);

				if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to create framebuffer object!\n");
				}
			}
			~IntermediateTexture() {
				glDeleteFramebuffers(1, &fb);
				glDeleteTextures(1, &tex);
				glDeleteRenderbuffers(1, &db);
			}
			void setToGenerate() {
				glBindFramebuffer(GL_FRAMEBUFFER, fb);
				glViewport(0, 0, width, height);
			}
			void setToRead() {
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, width, height);
			}
	};
	
	template<uint64_t size, int bufType>
	class GeneralBuffer {
			uint8_t cpuData[size] = {};
			GLuint handle = 0;
			GLuint blkIdx = 0;
		public:
			explicit GeneralBuffer(GLuint idx) : blkIdx(idx) {
				glGenBuffers(1, &handle);
				glBindBuffer(bufType, handle);
				glBufferData(bufType, size, cpuData, GL_DYNAMIC_DRAW);
				glBindBufferBase(bufType, blkIdx, handle);
				glBindBuffer(bufType, 0);
			}
			~GeneralBuffer() {
				glDeleteBuffers(1, &handle);
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