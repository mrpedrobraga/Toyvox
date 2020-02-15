
#include "sdlc.hpp"
#include "shader.hpp"
#include "dynamics.hpp"

using namespace tvx;

static constexpr uint_fast64_t unifBufSize = 512, maxVoxLvl = 8, resX = 400, resY = 300, dspX = 800, dspY = 600;
static const char *vertCover = "cover.vert", *fragOct = "oct.frag", *fragCover = "cover.frag";
static constexpr glm::vec3 startPos = glm::vec3(0.3, 0.6, 0.1);

int main(int argc, char **argv) {
	SdlContext sdlc("Toyvox Octree Rendering Test", dspX, dspY);

	ScreenCoveringTriangle tri;
	CubeMap skyTex("assets/cubemaps/miramar.png");
	IntermediateTexture<GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST> irmtex(resX, resY); // GL_RGB565 ?
	GeneralBuffer<unifBufSize, GL_UNIFORM_BUFFER> globals(0);
	std::unique_ptr<Voctree<maxVoxLvl>> voctree = std::make_unique<Voctree<maxVoxLvl>>();
	
	voctree->updateGpu(0);
	skyTex.use(1);
	
	GLuint shaderOctree = shaderLoadFile(vertCover, fragOct);
	GLuint shaderPost = shaderLoadFile(vertCover, fragCover);
	bool reloadShader = false, isQuitRequested = false; // MIDDLE-CLICK ANYWHERE IN WINDOW TO RELOAD SHADER FROM FILE
	Subscription reloadSub("mouse_down_middle", [&reloadShader] () -> void { reloadShader = true; });
	Subscription quitSub("key_down_escape", [&isQuitRequested] () -> void { isQuitRequested = true; }); // ESC EXITS

	Dynamics<maxVoxLvl> dynamics;
	Player<maxVoxLvl> player(startPos);
	player.setAspect(static_cast<float>(sdlc.getWindowWidth()) / static_cast<float>(sdlc.getWindowHeight()));
	
	sdlc.getDeltaTime(); // discard time since program start
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
		
		dynamics.tick(dt, player, *voctree);
		
		glm::mat4 uniformPackage;
		uniformPackage[0] = glm::vec4(resX, resY, time, dt);
		uniformPackage[1] = player.getPos();
		uniformPackage[2] = player.getRot();
		uniformPackage[3] = player.getCtrl();
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
		player.refresh(dt);
	}
	return 0;
}