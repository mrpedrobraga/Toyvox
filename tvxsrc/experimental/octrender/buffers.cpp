
#include "buffers.hpp"

namespace tvx {

	ScreenCoveringTriangle::ScreenCoveringTriangle() {
		float points[] = {-1.f, 3.f, 0.0f, -1.f, -1.f, 0.0f, 3.f, -1.f, 0.0f};
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, 9 * sizeof( float ), points, GL_STATIC_DRAW); // copy into bound OpenGL buffer
			glEnableVertexAttribArray(0);                                // enable input variables from bound VAO for shader
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr); // describe buffer's data layout. stored in VAO
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	GLuint ScreenCoveringTriangle::getVao() const {
		return vao;
	}
	CubeMap::CubeMap(const std::string &relPath, bool flip) : cubeTex(relPath, Texture::CUBE, flip) { }
	void CubeMap::use(GLenum unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex.get());
	}
}