#pragma once
#include <string.h>
#include <stdio.h>
#include <map>
#include <algorithm>

using namespace std;

/* For now, using a fixed value! */
#define MAX_ENTITIES 5000

/* Creating my own typenames for better understanding (and usage) of the code. */
using EntityUID = unsigned int;
using EntityType = unsigned int;
using ComponentType = string;

/* UTIL FUNCTIONS */
void log(string message)
{
	cout << message << endl;
}

void err(string message)
{
	cerr << message << endl;
}
/* END OF UTIL FUNCTIONS */

namespace tvx
{

	struct Component
	{
		//Contains data inside. Only here to be inherited later.
	};

	class ComponentSet
	{
		//Contains a set of 'childs' of 'Component' inside. Only here to be inherited later.
	};

	struct ComponentHandler //Handles all the component sets of a scene
	{
	private:
		map<ComponentType, ComponentSet*> component_sets;
	public:
		void add(ComponentType type, ComponentSet set)
		{
			component_sets[type] = &set;
		}

		void remove(ComponentType type)
		{
			component_sets.erase(type);
		}

		ComponentSet ctype(ComponentType type)
		{
			return *component_sets[type];
		}
	};

	//A single set handler, because there's only one *type* of entity. To give labels to entity, add the 'TypeTag' component.
	struct EntityHandler
	{
	private:
		//All the valied entity UIDs, densely packed for easy traversal.
		//Every time an entity is removed, the last entity fills the blank space.
		EntityUID entities[MAX_ENTITIES] = {0};
		size_t entity_count = 0;
		size_t next_id = 1;

		//If there's already an entity with that index, increase until there's not.
		//Sometimes you'll be in situations like this:
		//	entities = {0, 1, 4, 5, 6}
		//In this case, if next_id is, for example, 3, it's not a problem!
		void update_nid()
		{
			while(find(begin(entities), end(entities), next_id) != end(entities))
			{
				next_id++;
			}
		}
	public:
		EntityUID create()
		{
			//If the last index isn't free, so we've reached the entity limited
			//After all, it's densely packed data, there are no holes anywhere inside...
			if(entities[MAX_ENTITIES-1] != 0)
			{
				err("Toyvox Warning: ENTITY LIMIT REACHED");
				return 0;
			}

			entities[entity_count] = next_id;

			entity_count += 1;
			next_id += 1;
			update_nid();

			return next_id - 1;
		}

		//Note that here you use the index that create() gave you, not the entity UID!
		void destroy(EntityUID* entity)
		{
			size_t position = *find(begin(entities), end(entities), *entity);

			entities[position] = entities[entity_count];						//Move the last entity to this entity's place. Nice.
			entities[entity_count] = 0;

			if(*entity < next_id) 	//Set the next_id to fill in the hole next time create() is called.
				next_id = *entity;   //Of course you're not gonna change next_id if entity is bigger, let's not leave any holes behind.
			entity_count--;
			*entity=0;
		}
	};
	
	/*
					SYSTEM:
		An abstract class.
		Can do things to components.
		Affects all "objects" but only ever
		touches the components it needs to.
	*/
	class System
	{
		System()
		{}
		virtual ~System()=0;

		/* A system's tick function will be overriden by the actual system. */
		virtual int tick() = 0;
	};
	System::~System(){};

	/* 
					SCENE:
		Has the handlers that handle all
		systems, components, entities.
		With it, you can make rooms/levels.

		You can have scenes talking to each
		other easily, with 'global variables'.
	*/

	class Scene
	{
	public:

		ComponentHandler component_handler;	
		EntityHandler entity_handler;	

		Scene(char* title)
		{
			name = title;
		}

		~Scene()
		{}

		inline char* get_title()
		{
			return strdup(name);
		}

		EntityUID create_entity() {
			return entity_handler.create();
		}

		void destroy_entity(EntityUID* ent) {
			entity_handler.destroy(ent);
		}
	private:
		char* name;
	};

	/*
					COMPONENT HANDLER:
		Handles all entites' properties in a scene.
	*/
}