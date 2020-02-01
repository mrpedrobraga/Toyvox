
#pragma once

#include "tvxutil.h"

/********
 * FIX APPLIED: Use this debug function to figure out what's going wrong with opengl when it crashes
 ********/

#ifdef NDEBUG // Release build
static constexpr uint32_t CONTEXT_FLAGS = 0;
	static void setupOpenglDebugCallback() { }
#else // Debug Build
static constexpr uint32_t CONTEXT_FLAGS = SDL_GL_CONTEXT_DEBUG_FLAG;
// openglCallbackFunction found at https://bcmpinc.wordpress.com/2015/08/21/debugging-since-opengl-4-3/
static void APIENTRY openglCallbackFunction(
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar *message,
			const void *userParam
) {
	(void) source;
	(void) type;
	(void) id;
	(void) severity;
	(void) length;
	(void) userParam;
	fprintf(stderr, "%s\n", message);
	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		fprintf(stderr, "Aborting...\n");
		abort();
	}
}
static void setupOpenglDebugCallback() {
	// Enable the debug callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openglCallbackFunction, nullptr);
	glDebugMessageControl(
				GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true
	);
}
#endif