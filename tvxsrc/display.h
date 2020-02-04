#include <iostream>
#include "tvxutil.h"
#include "tvx_glutil.h"
#include "render.h"

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

          program = program_from_file(vertex_shader, pixel_shader);
	    }

	    bool is_closed()
	    {
	        return m_is_closed;
	    }

	    void update()
	    {
					glUseProgram(program);

	        SDL_GL_SwapWindow( m_window );
	        SDL_Event e;
	        while( SDL_PollEvent( &e ) )
	        {
	            if( e.type == SDL_QUIT )
	            {
	                m_is_closed = true;
	            }
	        }
	    }

	    ~Display()
	    {
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
	    SDL_GLContext m_gl_context;
			GLuint program;
			GLuint VAO, VBO;
	    bool m_is_closed;
			float vertices* = {	-0.5, -0.5, 0.0,
													+0.5, -0.5, 0.0,
													+0.5, +0.5, 0.0,
													-0.5, +0.5, 0.0		};
	};
}
