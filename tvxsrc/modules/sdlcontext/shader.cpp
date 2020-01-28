
#include "shader.hpp"

static constexpr int MAX_SHADER_SZ = 65535;

namespace tvx {
	
	GLuint shaderLoadString(const char *vsString, const char *fsString) {
		if (!vsString || !fsString) {
			SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Bad arguments passed to shaderLoadString");
			exit(106);
		}

		GLuint shaderProgram = glCreateProgram();
		GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
		GLuint fsHandle = glCreateShader(GL_FRAGMENT_SHADER);
		{
			glShaderSource(vsHandle, 1, &vsString, nullptr);
			glCompileShader(vsHandle);
			int params = -1;
			glGetShaderiv(vsHandle, GL_COMPILE_STATUS, &params);
			if (GL_TRUE != params) {
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "ERROR: vertex shader index %u did not compile\n", vsHandle);
				const int maxLength = 2048;
				int actualLength = 0;
				char slog[2048];
				glGetShaderInfoLog(vsHandle, maxLength, &actualLength, slog);
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "shader info log for GL index %u:\n%s\n", vsHandle, slog);

				glDeleteShader(vsHandle);
				glDeleteShader(fsHandle);
				glDeleteProgram(shaderProgram);
				return 0;
			}
		}

		{
			glShaderSource(fsHandle, 1, &fsString, nullptr);
			glCompileShader(fsHandle);
			int params = -1;
			glGetShaderiv(fsHandle, GL_COMPILE_STATUS, &params);
			if (GL_TRUE != params) {
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "ERROR: fragment shader index %u did not compile\n", fsHandle);
				const int maxLength = 2048;
				int actualLength = 0;
				char slog[2048];
				glGetShaderInfoLog(fsHandle, maxLength, &actualLength, slog);
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "shader info log for GL index %u:\n%s\n", fsHandle, slog);

				glDeleteShader(vsHandle);
				glDeleteShader(fsHandle);
				glDeleteProgram(shaderProgram);
				return 0;
			}
		}

		glAttachShader(shaderProgram, fsHandle);
		glAttachShader(shaderProgram, vsHandle);
		{
			glLinkProgram(shaderProgram);
			glDeleteShader(vsHandle);
			glDeleteShader(fsHandle);
			int params = -1;
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &params);
			if (GL_TRUE != params) {
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "ERROR: could not link shader program GL index %u\n", shaderProgram);
				const int maxLength = 2048;
				int actualLength = 0;
				char plog[2048];
				glGetProgramInfoLog(shaderProgram, maxLength, &actualLength, plog);
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "program info log for GL index %u:\n%s", shaderProgram, plog);

				glDeleteProgram(shaderProgram);
				return 0;
			}
		}
		if ( ! shaderProgram) { SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to create shader!\n"); }
		return shaderProgram;
	}

	GLuint shaderLoadFile(const char *vsFilename, const char *fsFilename) {
		if (!vsFilename || !fsFilename) {
			SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Bad arguments passed to shaderLoadFile");
			exit(105);
		}

		char vsString[MAX_SHADER_SZ];
		char fsString[MAX_SHADER_SZ];
		vsString[0] = fsString[0] = '\0';
		{
			FILE *fp = fopen(("assets/shaders/" + std::string(vsFilename)).c_str(), "r");
			if (!fp) {
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Could not open vertex shader file `%s`\n", vsFilename);
				return 0;
			}
			size_t count = fread(vsString, 1, MAX_SHADER_SZ - 1, fp);
			if (count >= MAX_SHADER_SZ) { SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Shader size exceeded MAX_SHADER_SZ"); }
			vsString[count] = '\0';
			fclose(fp);
		}
		{
			FILE *fp = fopen(("assets/shaders/" + std::string(fsFilename)).c_str(), "r");
			if (!fp) {
				SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Could not open fragment shader file `%s`\n", fsFilename);
				return 0;
			}
			size_t count = fread(fsString, 1, MAX_SHADER_SZ - 1, fp);
			if (count >= MAX_SHADER_SZ) { SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Shader size exceeded MAX_SHADER_SZ");}
			fsString[count] = '\0';
			fclose(fp);
		}

		return shaderLoadString(vsString, fsString);
	}

	void shaderReloadFile(GLuint *program, const char *vsFilename, const char *fsFilename) {
		if (!program || !vsFilename || !fsFilename) {
			SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Bad arguments passed to shaderReloadFile");
			exit(104);
		}
		GLuint reloadedProgram = shaderLoadFile(vsFilename, fsFilename);
		if (reloadedProgram) {
			glDeleteProgram(*program);
			*program = reloadedProgram;
		}
	}
}
