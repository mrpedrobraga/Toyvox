
#include "sdlc.hpp"
#include "shader.hpp"
#include "buffers.hpp"
#include "topics.hpp"
#include "octren.hpp"
#include "camera.hpp"

using namespace tvx;

static constexpr uint64_t voxelBufSize = 65536; // Minimum spec UBO size
static constexpr int voxelBufType = GL_UNIFORM_BUFFER;
static constexpr uint64_t unifBufSize = 512;
static constexpr int unifBufType = GL_UNIFORM_BUFFER;
static const char *vert = "cover.vert";
static const char *fragDisp = "cooler.frag";

int main(int argc, char **argv) {
	SdlContext sdlc("Toyvox Octree Rendering Test");
	GeneralBuffer<voxelBufSize, voxelBufType>::reportUboSupport();
	
	GLuint shaderDisp = shaderLoadFile(vert, fragDisp);
	bool reloadShader = false, isQuitRequested = false; // CLICK ANYWHERE IN WINDOW TO RELOAD SHADER FROM FILE
	Subscription reloadSub("mouse_down_left", [&reloadShader] () -> void { reloadShader = true; });
	Subscription quitSub("key_down_escape", [&isQuitRequested] () -> void { isQuitRequested = true; }); // ESC EXITS

	ScreenCoveringTriangle tri;
	GeneralBuffer<voxelBufSize, voxelBufType> voxels(0);
	GeneralBuffer<unifBufSize, unifBufType> globals(1);
	
	FreeCamera cam(glm::vec3(0.3, 0.6, 0.3));
	cam.setAspect(static_cast<float>(sdlc.getWindowWidth()) / static_cast<float>(sdlc.getWindowHeight()));

	uint_fast32_t colorAxisDivisor = 2;//4
	for (uint_fast64_t i = 0; i < voxels.getCapacity<VoxelDword>(); ++i) {
		glm::uvec3 pos = VoxelDword::demortonize(i);
		VoxelDword voxel;
		voxel.setIsFilled(! (i % 5));
		voxel.setRed(pos.x / colorAxisDivisor);
		voxel.setGreen(pos.z / colorAxisDivisor);
		voxel.setBlue(pos.y / colorAxisDivisor);
		voxels.writeToCpu<VoxelDword>(i, voxel);
	}
	voxels.sendToGpu();
	
	float time = 0.f;
	while (sdlc.pollEvents(isQuitRequested)) {
		SdlContext::pollStates();
		float dt = sdlc.getDeltaTime();
		time += dt;
		
		if (reloadShader) {
			reloadShader = false;
			shaderReloadFile(&shaderDisp, vert, fragDisp);
		}
		
		glm::mat4 uniformPackage;
		uniformPackage[0] = glm::vec4(sdlc.getWindowWidth(), sdlc.getWindowHeight(), time, dt);
		uniformPackage[1] = cam.getPos();
		uniformPackage[2] = cam.getRot();
		uniformPackage[3] = cam.getCtrl();
		globals.writeToCpu<glm::mat4>(0, uniformPackage);
		globals.sendToGpu();
		
		glBindVertexArray(tri.getVao());
		glUseProgram(shaderDisp);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
		
		sdlc.swapWindow();
		cam.refresh(dt);
	}
	return 0;
}