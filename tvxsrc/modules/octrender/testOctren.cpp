
#include <SDL_log.h>
#include "octren.hpp"
#include "sdlc.hpp"
#include "shader.hpp"
#include "topics.hpp"

using namespace tvx;

void sendTriangleToGpu(GLuint &vao, GLuint &vbo);

int main(int argc, char **argv) {
	SdlContext sdlc("Toyvox Octree Rendering Test");
	SDL_Log("Octree Rendering Test");

	struct SsboData {
		float camera_position[4];
		float light_position[4];
		float light_diffuse[4];
	} ssboData = {};

	GLuint triVao = 0;
	GLuint triVbo = 0;
	sendTriangleToGpu(triVao, triVbo);
	
	GLuint ssbo = 0;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ssboData), &ssboData, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// CLICK IN WINDOW TO RELOAD SHADER FROM FILE
	GLuint shader = 0;
	bool reloadShader = true;
	Subscription reloadSub("mouse_down_left", [&reloadShader] () -> void {
		reloadShader = true;
	});
	
	while (sdlc.pollEvents()) {
		SdlContext::pollStates();
		
		if (reloadShader) {
			reloadShader = false;
			if (shader) {
				shaderReloadFile(&shader, "oct.vert", "oct.frag");
			} else {
				shader = shaderLoadFile("oct.vert", "oct.frag");
			}
		}

		sdlc.clearColor();
		float dt = sdlc.getDeltaTime();

		{ // write to ssbo
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			GLvoid *p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
			memcpy(p, &ssboData, sizeof(ssboData));
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		}

		{ // draw triangle
			glUseProgram(shader);
			glBindVertexArray(triVao);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
			glUseProgram(0);
		}
		
		sdlc.swapWindow();
	}
	return 0;
}

void sendTriangleToGpu(GLuint &vao, GLuint &vbo) {
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