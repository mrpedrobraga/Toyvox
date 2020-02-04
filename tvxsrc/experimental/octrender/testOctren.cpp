
#include "sdlc.hpp"
#include "shader.hpp"
#include "buffers.hpp"
#include "topics.hpp"
#include "octren.hpp"
#include "camera.hpp"

using namespace tvx;

static constexpr uint_fast64_t unifBufSize = 512, unifBufType = GL_UNIFORM_BUFFER, maxVoxLvl = 4;
static const char *vert = "cover.vert", *frag = "oct.frag";

int main(int argc, char **argv) {
	SdlContext sdlc("Toyvox Octree Rendering Test");
	
	GLuint shaderDisp = shaderLoadFile(vert, frag);
	bool reloadShader = false, isQuitRequested = false; // CLICK ANYWHERE IN WINDOW TO RELOAD SHADER FROM FILE
	Subscription reloadSub("mouse_down_left", [&reloadShader] () -> void { reloadShader = true; });
	Subscription quitSub("key_down_escape", [&isQuitRequested] () -> void { isQuitRequested = true; }); // ESC EXITS

	ScreenCoveringTriangle tri;
	GeneralBuffer<unifBufSize, unifBufType> globals(1);
	Voxtree<maxVoxLvl> voxtree(0);
	voxtree.updateGpu();
	
	FreeCamera cam(glm::vec3(0.3, 0.6, 0.1));
	cam.setAspect(static_cast<float>(sdlc.getWindowWidth()) / static_cast<float>(sdlc.getWindowHeight()));
	
	float time = 0.f;
	while (sdlc.pollEvents(isQuitRequested)) {
		SdlContext::pollStates();
		float dt = sdlc.getDeltaTime();
		time += dt;
		if (reloadShader) {
			reloadShader = false;
			shaderReloadFile(&shaderDisp, vert, frag);
		}
		glm::mat4 uniformPackage;
		uniformPackage[0] = glm::vec4(sdlc.getWindowWidth(), sdlc.getWindowHeight(), time, dt);
		uniformPackage[1] = cam.getPos();
		uniformPackage[2] = cam.getRot();
		uniformPackage[3] = cam.getCtrl();
		uniformPackage[3].w = maxVoxLvl;
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