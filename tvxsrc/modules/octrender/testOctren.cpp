
#include "sdlc.hpp"
#include "shader.hpp"
#include "buffers.hpp"
#include "topics.hpp"
#include "octren.hpp"

using namespace tvx;

static constexpr uint64_t voxelBufSize = 65536; // Minimum spec UBO size
static constexpr int voxelBufType = GL_UNIFORM_BUFFER;
static constexpr uint64_t unifBufSize = 256;
static constexpr int unifBufType = GL_UNIFORM_BUFFER;
static const char *vert = "cover.vert";
static const char *frag = "oct.frag";

int main(int argc, char **argv) {
	SdlContext sdlc("Toyvox Octree Rendering Test");
	GpuBuffer<voxelBufSize, voxelBufType>::reportUboSupport();
	
	GLuint shader = shaderLoadFile(vert, frag);;
	bool reloadShader = false, isQuitRequested = false; // CLICK ANYWHERE IN WINDOW TO RELOAD SHADER FROM FILE
	Subscription reloadSub("mouse_down_left", [&reloadShader] () -> void { reloadShader = true; });
	Subscription quitSub("key_down_space", [&isQuitRequested] () -> void { isQuitRequested = true; }); // SPACE EXITS

	ScreenCoveringTriangle tri;
	GpuBuffer<voxelBufSize, voxelBufType> voxels(0);
	GpuBuffer<unifBufSize, unifBufType> globals(1);
	
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

		uint_fast32_t colorAxisDivisor = pow(voxels.getCapacity<VoxelDword>(), 1 / 3);
		for (int i = 0; i < voxels.getCapacity<VoxelDword>(); ++i) {
			glm::uvec3 pos = VoxelDword::demortonize(i);
			VoxelDword voxel;
			voxel.setRed(pos.x / colorAxisDivisor);
			voxel.setGreen(pos.z / colorAxisDivisor);
			voxel.setBlue(pos.y / colorAxisDivisor);
			voxels.writeToCpu<VoxelDword>(i, voxel);
		}
		voxels.sendToGpu();
		
		globals.writeToCpu<glm::vec4>(0, glm::vec4(sdlc.getWindowWidth(), sdlc.getWindowHeight(), time, dt));
		globals.sendToGpu();
		
		glUseProgram(shader);
		glBindVertexArray(tri.getVao());
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
		
		sdlc.swapWindow();
	}
	return 0;
}