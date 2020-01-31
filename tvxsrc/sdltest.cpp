#include "tvxcore.h"

SDL_Window* window;
SDL_GLContext glcontext;

const char *vertexShaderSource =	"#version 330 core\n"
								    "layout (location = 0) in vec3 vpos;\n"
								    "void main()\n"
								    "{\n"
								    "   gl_Position = vec4(vec3(vpos), 1.0);\n"
								    "}\0";

const char *pixelShaderSource =		"#version 330 core\n"
								    "out vec4 FragColor;\n"
								    "void main()\n"
								    "{\n"
								    "   FragColor = vec4(0.2, 0.6, 0.9, 1.0);\n"
								    "}\0";

int main (int argc, char** argv) {

	cout << "1\n";

	SDL_Init(SDL_INIT_VIDEO);

	cout << "2\n";

	SDL_GL_LoadLibrary(NULL);

	// Request an OpenGL 4.5 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Check OpenGL properties
	printf("OpenGL loaded\n");
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	// Use v-sync
	SDL_GL_SetSwapInterval(1);

	float vertices[] = {
    	-0.5f, -0.5f, 0.0f,
     	 0.5f, -0.5f, 0.0f,
    	 0.0f,  0.5f, 0.0f
	}; 

	window = SDL_CreateWindow(
			"Toyvox Game",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			640,                               // width, in pixels
			360,                               // height, in pixels
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE                  // flags - see below
	);

	glcontext = SDL_GL_CreateContext(window);

	glClearColor(0, 1, 1, 1);

	unsigned int VBO;
	glGenBuffers(1, &VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	unsigned int pixelShader;
	pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(pixelShader, 1, &pixelShaderSource, NULL);
	glCompileShader(pixelShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, pixelShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(pixelShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);  

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	SDL_Event e;

	while(SDL_WaitEvent(&e)) {
		glDrawArrays(GL_TRIANGLES, 0, 3);

		if (e.type = SDL_QUIT) break;
	}
}