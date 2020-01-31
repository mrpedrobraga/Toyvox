#pragma once
#include "tvxutil.h"
#include "objects.h"

/* Some default components and component sets! */

namespace tvx
{

	/* Type tags, in case you want to differenciate types of entities. */
	struct TypeTag
	{
	private:
		char* value;
	public:
		char* get_value()
		{
			return value;
		}

		void set_value(const char* new_value)
		{
			set_value(strdup(new_value));
		}

		void set_value(char* new_value)
		{
			value = new_value;
		}

		TypeTag()
		{
			value = strdup("");
		}

		TypeTag(const char* new_value)
		{
			value = strdup(new_value);
		}

		TypeTag(char* new_value)
		{
			value = new_value;
		}

	};

	enum CameraType {
		TVX_PERSPECTIVE_CAMERA,
		TVX_ORTOGRAPHIC_CAMERA,
		TVX_2D_CAMERA,
	};

	//This class hold many components on a list!
	template<class Type>
	struct ComponentSet //Also densely packed! See EntityHandler to see how this works!
	{
	private:

		EntityUID entities[MAX_ENTITIES] = {0};
		Type cset[MAX_ENTITIES];
		size_t entity_count = 0;
		size_t next_id = 1;

		void update_nid()
		{
			while(find(begin(entities), end(entities), next_id) != end(entities))
			{
				next_id++;
			}
		}

	public:

		void set(EntityUID e, Type value)
		{
			if(entities[MAX_ENTITIES-1] != 0)
			{
				err("Toyvox Warning: ENTITY LIMIT REACHED");
				return;
			}

			entities[entity_count] = next_id;
			cset[e] = value;
			entity_count += 1;
			next_id += 1;
			update_nid();
		}

		Type& is_assigned_to(EntityUID& e)
		{
			auto it = std::find(begin(entities), end(entities), e);
			return (it != std::end(entities));
		}

		Type& of(EntityUID e)
		{
			auto it = std::find(begin(entities), end(entities), e);
			return (cset[std::distance(entities, it)]);
		}

		void apply(void (*tick)(Type&))
		{
			for(size_t i = 0; i < entity_count; i++)
				(*tick)(cset[i]);
		}

		//Note that here you use the index that create() gave you, not the entity UID!
		void remove(EntityUID& entity)
		{
			size_t position = *find(begin(entities), end(entities), entity);

			entities[position] = entities[entity_count];						//Move the last entity to this entity's place. Nice.
			cset[position] = cset[entity_count];
			entities[entity_count] = 0;
			cset[entity_count] = 0;

			if(entity < next_id) 	//Set the next_id to fill in the hole next time create() is called.
				next_id = entity;   //Of course you're not gonna change next_id if entity is bigger, let's not leave any holes behind.
			entity_count--;
			entity=0;
		}

		ComponentSet()
		{}
	};
}