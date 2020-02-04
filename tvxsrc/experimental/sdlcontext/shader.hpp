#pragma once

#include "extern.hpp"

namespace tvx {

	GLuint shaderLoadString(const char *vsString, const char *fsString);
	GLuint shaderLoadFile(const char *vsFilename, const char *fsFilename);
	void shaderReloadFile(GLuint *program, const char *vsFilename, const char *fsFilename);
}
