
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
			Texture(const std::string &textureFile, RequestedType requestedType);
			virtual ~Texture();
			GLuint get();

		private:
			GLuint tex = 0;
			LoadedType status = INVALID;

			void loadSingleTexture(const std::string &textureFile);
			void loadCubeTexture(const std::string &textureFileBaseName);
			static int loadCubeSide(GLenum whichSide, const std::string &fileName);
	};
}
