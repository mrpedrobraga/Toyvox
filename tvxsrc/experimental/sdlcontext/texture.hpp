
#pragma once

#include "extern.hpp"

namespace tvx {
	class Texture {

		public:
			enum RequestedType {
				SINGLE, CUBE
			};
			enum LoadedType {
				INVALID, SINGLE_RGB, SINGLE_RGBA, CUBE_RGB
			};
			Texture(const std::string &textureFile, RequestedType requestedType, bool flip = false);
			virtual ~Texture();
			GLuint get();

		private:
			GLuint tex = 0;
			LoadedType status = INVALID;

			void loadSingleTexture(const std::string &textureFile);
			void loadCubeTexture(const std::string &textureFileBaseName, bool flip = false);
			static int loadCubeSide(GLenum whichSide, const std::string &fileName);
	};
}
