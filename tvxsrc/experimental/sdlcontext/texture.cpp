
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#include "stb_image.h"
#include "texture.hpp"

namespace tvx {

	void Texture::loadSingleTexture(const std::string &textureFile) {
		if (textureFile.empty()) {
			return;
		}
		int x, y, n;
		stbi_set_flip_vertically_on_load(true);
		uint8_t* data = stbi_load(textureFile.c_str(), &x, &y, &n, 0);
		if (data) {
			GLint oglFormat = 0;
			switch (n) {
				case 3: {
					status = SINGLE_RGB;
					oglFormat = GL_RGB;
				} break;
				case 4: {
					status = SINGLE_RGBA;
					oglFormat = GL_RGBA;
				} break;
				default: {
					SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Unsupported channels in texture file '%s'.", textureFile.c_str());
					stbi_image_free(data);
					return;
				}
			}
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(
						GL_TEXTURE_2D,     // target
						0,                 // level
						oglFormat,         // internal format
						x,                 // width
						y,                 // height
						0,                 // border
						(GLenum)oglFormat, // format
						GL_UNSIGNED_BYTE,  // type
						data               // data
			);

			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		} else {
			SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to load texture file '%s'.", textureFile.c_str());
		}
		stbi_image_free(data);
	}

	void Texture::loadCubeTexture(const std::string &textureFileBaseName) {

		// split file an extension names
		size_t lastDotIndex = textureFileBaseName.find_last_of('.');
		std::string rawName = textureFileBaseName.substr(0, textureFileBaseName.find_last_of('.'));
		std::string extName = textureFileBaseName.substr(lastDotIndex, textureFileBaseName.size());

		// generate true file names for separate images (one for each side of cube)
		std::string front   = std::string(rawName + "/negz" + extName);
		std::string back    = std::string(rawName + "/posz" + extName);
		std::string bottom  = std::string(rawName + "/negy" + extName);
		std::string top     = std::string(rawName + "/posy" + extName);
		std::string left    = std::string(rawName + "/negx" + extName);
		std::string right   = std::string(rawName + "/posx" + extName);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
		
		int r;
#   define CONTINUE_IF_SUCCESS(s, f) r = loadCubeSide(s, f); if (r) return;
			CONTINUE_IF_SUCCESS(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
			CONTINUE_IF_SUCCESS(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
			CONTINUE_IF_SUCCESS(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
			CONTINUE_IF_SUCCESS(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
			CONTINUE_IF_SUCCESS(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
			CONTINUE_IF_SUCCESS(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
#   undef CONTINUE_IF_SUCCESS
		
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	int Texture::loadCubeSide(GLenum whichSide, const std::string &fileName) {
		int x, y, n;
		int forceChannels = 4;
		unsigned char *imageData = stbi_load(fileName.c_str(), &x, &y, &n, forceChannels);
		if (!imageData) {
			SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Failed to load texture file: '%s'.\n", fileName.c_str());
			return 1;
		}
		if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "Texture not power of two: '%s'.\n", fileName.c_str());
			return 1;
		}
		glTexImage2D(whichSide, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		stbi_image_free(imageData);
		return 0;
	}

	Texture::Texture(const std::string &textureFile, RequestedType requestedType) {
		switch (requestedType) {
			case SINGLE: {
				SDL_Log("Loading texture: %s\n", textureFile.c_str());
				loadSingleTexture(textureFile);
			} break;
			case CUBE: {
				SDL_Log("Loading cube map: %s\n", textureFile.c_str());
				loadCubeTexture(textureFile);
			} break;
			default: break;
		}
	}

	Texture::~Texture() {
		glDeleteTextures(1, &tex);
	}

	GLuint Texture::get() {
		return tex;
	}
}
