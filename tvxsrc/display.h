#include <iostream>
#include "tvxutil.h"
#include "tvx_glutil.h"
#include "objects.h"
#include "voxels.h"
#include "textures.h"

namespace tvx {
	class Display
	{
	public:
	    Display(std::string& game_title, glm::ivec2& resolution, GLuint& window_flags)
	        : m_is_closed( false )
	    {
	        SDL_Init( SDL_INIT_EVERYTHING );

	        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	        SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
	        SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
	        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
	        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	        m_window = SDL_CreateWindow
	            (
	            game_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	            resolution.x, resolution.y,
	            window_flags
	            );
	        m_gl_context = SDL_GL_CreateContext( m_window );

					gladLoadGLLoader(SDL_GL_GetProcAddress);

					// Load OpenGL pointers and print info
					printf("OpenGL loading...\n");

					printf("Vendor:          %s\n", glGetString(GL_VENDOR));
					printf("Renderer:        %s\n", glGetString(GL_RENDERER));
					printf("Version OpenGL:  %s\n", glGetString(GL_VERSION));
					printf("Version GLSL:    %s\n\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
					fflush(stdout); fflush(stderr);
	    }

	    bool is_closed()
	    {
	        return m_is_closed;
	    }

			void init()
			{
				test();
				define_shaders();
				m_voxelBuffer.init();
				program = program_from_file(vertex_shader_path, pixel_shader_path);

				float quad[] =
				{
					-1.0f,  1.0f,	// v0 - top left corner
					-1.0f, -1.0f,	// v1 - bottom left corner
					 1.0f,  1.0f,	// v2 - top right corner
					 1.0f, -1.0f	// v3 - bottom right corner
				};

				// Create the vertex array object for the full screen quad.

				glGenVertexArrays(1, &m_vertexArrayObject);
				glBindVertexArray(m_vertexArrayObject);

				// Create the Vertex Buffer Object for the full screen quad.

				glGenBuffers(1, &m_vertexBufferObject);
				glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

				// Map the generic vertex attributes used by the full screen quad's shader
				// program to the full screen quad's vertex data stored in the vertex
				// buffer object.

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
				glEnableVertexAttribArray(0);

				glBindVertexArray(0);

				// Set up the voxel Buffer
				m_voxelBuffer.use(0);
			}

			void sendUniforms()
			{
				// time
				glUniform1f(glGetUniformLocation(program, "time"), wall_clock_time());

				// camera_rotation_matrix
				glm::vec3 camera_rotation = glm::vec3(0.0, 0.0, 0.785);

				glm::mat4x4 mat_rotation = 	glm::rotate(glm::mat4x4(1), camera_rotation.z, glm::vec3(0,0,1)) *
																		glm::rotate(glm::mat4x4(1), camera_rotation.y, glm::vec3(0,1,0)) *
																		glm::rotate(glm::mat4x4(1), camera_rotation.x, glm::vec3(1,0,0));

				glUniformMatrix4fv(glGetUniformLocation(program, "camera_rotation"), 1, GL_FALSE, &mat_rotation[0][0]);

				//The size of the model, in voxels.
				glUniform1f(glGetUniformLocation(program, "model_size"), m_modelSize);

				//The content of the octree, in dense tree data.
				//glUniform1uiv(glGetUniformLocation(program, "voxels"), sizeof(m_voxelBuffer)/sizeof(Voxel), (GLuint*) m_voxelBuffer);

				//m_voxelBuffer.sendToGpu();
			}

			void test()
			{
				for(size_t i = 0; i < m_modelSize; i++) for(size_t j = 0; j < m_modelSize; j++) for(size_t k = 0; k < m_modelSize; k++) {

					int a = 0;

					if(glm::distance(glm::vec3(i, j, k), glm::vec3(5, 5, 5)) >= 5.5) a = 255;

					const int index = i + j * m_modelSize + k * m_modelSize * m_modelSize;

					m_voxelBuffer.writeToCpu<glm::uint32_t>(index, getIntColour(glm::ivec4(i*255/m_modelSize, j*255/m_modelSize, k*255/m_modelSize, a)));
				}
			}

	    void update(std::shared_ptr<Scene>& scene)
	    {
	        SDL_Event e;
	        while( SDL_PollEvent( &e ) )
	        {

							glUseProgram(program);

							sendUniforms();

							SDL_Delay(16);

							if(scene->on_event) scene->on_event(e, *scene);

							switch (e.type)
	            {
								case SDL_QUIT:
	                m_is_closed = true;
									break;
								case SDL_KEYDOWN:
									if(scene->on_key_pressed) scene->on_key_pressed(e, *scene);
								case SDL_WINDOWEVENT:
									switch (e.window.event) {
										case SDL_WINDOWEVENT_RESIZED:
											SDL_GetWindowSize(m_window, &m_windowsize.x, &m_windowsize.y);
											glViewport(0, 0, m_windowsize.x, m_windowsize.y);
											//glUniform2uiv(glGetUniformLocation(program, "resolution"), sizeof(m_windowsize), (GLuint*)(m_windowsize));
											break;
										case SDL_WINDOWEVENT_FOCUS_GAINED:
											reload_program(&program, vertex_shader_path, pixel_shader_path);
											break;

									}
	            }


	        }

					//Draw everything!

					glBindVertexArray(m_vertexArrayObject);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
					glBindVertexArray(0);

					SDL_GL_SwapWindow( m_window );
	    }

	    ~Display()
	    {
				if (m_vertexArrayObject)
				{
					glDeleteVertexArrays(1, &m_vertexArrayObject);
					m_vertexArrayObject = 0;
				}

				if (m_vertexBufferObject)
				{
					glDeleteBuffers(1, &m_vertexBufferObject);
					m_vertexBufferObject = 0;
				}
				//glBindBuffer(0);
				glBindVertexArray(0);
				glUseProgram(0);
        SDL_GL_DeleteContext( m_gl_context );
        SDL_DestroyWindow( m_window );
        SDL_Quit();
	    }

	    Display( const Display& other )
	    {
	        //copy ctor
	    }

	    Display& operator=( const Display& rhs )
	    {
	        if( this == &rhs ) return *this; // handle self assignment
	        //assignment operator
	        return *this;
	    }

	private:
	    SDL_Window* m_window;
			glm::ivec2 m_windowsize;
			std::string m_windowtitle;
	    SDL_GLContext m_gl_context;
			GLuint program;
			GLuint VAO, VBO;
	    bool m_is_closed;
			GLuint m_vertexBufferObject, m_vertexArrayObject;

			BufferTexture<1331> m_voxelBuffer;
			int m_modelSize = 11;
	};
}
