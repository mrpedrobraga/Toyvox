
#include <SDL_log.h>
#include "octren.hpp"
#include "sdlc.hpp"
#include "shader.hpp"
#include "topics.hpp"
#include "morton.h"

using namespace tvx;

class GpuBuffer {
		static constexpr uint64_t size = 65536; // Minimum spec UBO size
		static constexpr int bufType = GL_UNIFORM_BUFFER;
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
			memcpy(p, cpuData, GpuBuffer::size);
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

void setUpVao(GLuint &vao, GLuint &vbo) {
	float points[] = { -1.f, 3.f, 0.0f, -1.f, -1.f, 0.0f, 3.f, -1.f, 0.0f };
	glGenVertexArrays( 1, &vao );
	glGenBuffers( 1, &vbo );
	glBindVertexArray(vao );
	glBindBuffer(GL_ARRAY_BUFFER, vbo );
	{
		glBufferData( GL_ARRAY_BUFFER, 9 * sizeof( float ), points, GL_STATIC_DRAW ); // copy into bound OpenGL buffer
		glEnableVertexAttribArray( 0 );                                // enable input variables from bound VAO for shaders
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr ); // describe buffer's data layout. stored in VAO
	}
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray( 0 );
}

static const char *vert = "cover.vert";
static const char *frag = "randOct.frag";
int main(int argc, char **argv) {
	SdlContext sdlc("Toyvox Octree Rendering Test");
	GpuBuffer::reportUboSupport();
	
	GLuint shader = shaderLoadFile(vert, frag);;
	bool reloadShader = false, isQuitRequested = false; // CLICK ANYWHERE IN WINDOW TO RELOAD SHADER FROM FILE
	Subscription reloadSub("mouse_down_left", [&reloadShader] () -> void { reloadShader = true; });
	Subscription quitSub("key_down_space", [&isQuitRequested] () -> void { isQuitRequested = true; }); // SPACE EXITS

	GLuint triVao = 0;
	GLuint triVbo = 0;
	setUpVao(triVao, triVbo);
	GpuBuffer voxels(0);
	GpuBuffer globals(1);
	
	float time = 0.f;
	while (sdlc.pollEvents(isQuitRequested)) {
		SdlContext::pollStates();
		float dt = sdlc.getDeltaTime();
		time += dt;
		
		if (reloadShader) {
			reloadShader = false;
			shaderReloadFile(&shader, vert, frag);
		}

		sdlc.clearColor();

		for (int i = 0; i < voxels.getCapacity<uint32_t>(); ++i) {
			uint_fast32_t x, y, z;
			libmorton::morton3D_32_decode(i, x, y, z);
			uint32_t colorByte = (x & 0xFFu) | (y << 8u & 0xFF00u) | (z << 16u & 0xFF0000u) | (0x01000000u);
			voxels.writeToCpu<uint32_t>(i, colorByte);
		}
		voxels.sendToGpu();
		
		globals.writeToCpu<glm::vec4>(0, glm::vec4(sdlc.getWindowWidth(), sdlc.getWindowHeight(), time, dt));
		globals.sendToGpu();
		
		glUseProgram(shader);
		glBindVertexArray(triVao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
		
		sdlc.swapWindow();
	}
	return 0;
}