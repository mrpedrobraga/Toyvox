
#include <SDL_log.h>
#include "sdlc.hpp"
#include "shader.hpp"

using namespace tvx;

extern const GLchar *vertexShaderSource;
extern const GLchar *fragmentShaderSource;
void setUpVao(GLuint &vao, GLuint &vbo);

int main(int argc, char **argv) {	
	SDL_Log("Triangle Rendering Test");
	SdlContext sdlc("Toyvox Triangle Rendering Test");

	GLuint triVao = 0;
	GLuint triVbo = 0;
	setUpVao(triVao, triVbo);
	
	GLuint shader = shaderLoadString(vertexShaderSource, fragmentShaderSource);
	if ( ! shader) { SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to create shaders!\n"); }
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	while (sdlc.pollEvents()) {
		sdlc.clearColor();
		float dt = sdlc.getDeltaTime();
		
		glUseProgram(shader);
		glBindVertexArray(triVao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
		
		sdlc.swapWindow();
	}
	return 0;
}

void setUpVao(GLuint &vao, GLuint &vbo) {
	float points[] = { 0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };
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
const GLchar *vertexShaderSource =
			"#version 420 core\n"
			"layout (location = 0) in vec3 position;"
			"void main()"
			"{"
			"gl_Position = vec4(position.x, position.y, position.z, 1.0);"
			"}";
const GLchar *fragmentShaderSource =
			"#version 420 core\n"
			"out vec4 color;"
			"void main()"
			"{"
			"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
			"}";