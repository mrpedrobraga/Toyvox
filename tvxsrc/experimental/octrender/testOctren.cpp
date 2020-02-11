
#include "sdlc.hpp"
#include "shader.hpp"
#include "buffers.hpp"
#include "topics.hpp"
#include "octren.hpp"
#include "camera.hpp"

using namespace tvx;

static constexpr uint_fast64_t unifBufSize = 512, maxVoxLvl = 7, resX = 800, resY = 600, dspX = 800, dspY = 600;
static const char *vertCover = "cover.vert", *fragOct = "oct.frag", *fragCover = "cover.frag";
static constexpr glm::vec3 startPos = glm::vec3(0.3, 0.6, 0.1);

int main(int argc, char **argv) {
	SdlContext sdlc("Toyvox Octree Rendering Test", dspX, dspY);

	ScreenCoveringTriangle tri;
	IntermediateTexture<GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST> irmtex(resX, resY); // GL_RGB565 ?
	GeneralBuffer<unifBufSize, GL_UNIFORM_BUFFER> globals(0);
	Voxtree<maxVoxLvl> voxtree;
	
	voxtree.updateGpu();
	
	GLuint shaderOctree = shaderLoadFile(vertCover, fragOct);
	GLuint shaderPost = shaderLoadFile(vertCover, fragCover);
	bool reloadShader = false, isQuitRequested = false; // CLICK ANYWHERE IN WINDOW TO RELOAD SHADER FROM FILE
	Subscription reloadSub("mouse_down_left", [&reloadShader] () -> void { reloadShader = true; });
	Subscription quitSub("key_down_escape", [&isQuitRequested] () -> void { isQuitRequested = true; }); // ESC EXITS
	
	FreeCamera cam(maxVoxLvl, startPos);
	cam.setAspect(static_cast<float>(sdlc.getWindowWidth()) / static_cast<float>(sdlc.getWindowHeight()));
	
	float time = 0.f;
	while (sdlc.pollEvents(isQuitRequested)) {
		SdlContext::pollStates();
		float dt = sdlc.getDeltaTime();
		time += dt;
		if (reloadShader) {
			reloadShader = false;
			shaderReloadFile(&shaderOctree, vertCover, fragOct);
			shaderReloadFile(&shaderPost, vertCover, fragCover);
		}
		glm::mat4 uniformPackage;
		uniformPackage[0] = glm::vec4(resX, resY, time, dt);
		uniformPackage[1] = cam.getPos();
		uniformPackage[2] = cam.getRot();
		uniformPackage[3] = cam.getCtrl();
		uniformPackage[3].x = dspX;
		uniformPackage[3].y = dspY;
		globals.writeToCpu<glm::mat4>(0, uniformPackage);
		globals.sendToGpu();
		
		glBindVertexArray(tri.getVao());
		glUseProgram(shaderOctree);
		irmtex.setToGenerate();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderPost);
		irmtex.setToRead(sdlc.getWindowWidth(), sdlc.getWindowHeight());
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
		
		sdlc.swapWindow();
		cam.refresh(dt);
	}
	return 0;
}