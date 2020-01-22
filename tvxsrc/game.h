#include <glm/glm.hpp>
#include "tvxutil.h"
#include <string.h>

namespace tvx {


	/* 					Game				 		*/
	/* 	The world basically contains which scene
		is the current. Also, some other 'global'
		settings.									*/

	struct Game
	{
	private:
		Scene* current_scene;
		text game_title = strdup("default");
		glm::ivec2 resolution;

		bool should_stop;
	public:
		Scene get_current_scene(){return *current_scene;}
		void set_current_scene(Scene* new_scene)
		{
			current_scene = new_scene;
			new_scene->on_load();
		}

		text get_title(){return game_title;}
		void set_title(text title){game_title = title;}

		glm::ivec2 get_resolution(){return resolution;}
		void set_resolution(glm::ivec2 res){resolution = res;}

		Game()
		{}

		void run()
		{
			should_stop = false;
			
		}

		void stop()
		{
			//Close Window
			should_stop = true;
		}
	};
}