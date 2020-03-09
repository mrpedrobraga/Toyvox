#pragma once
#include "tvxutil.h"
#include "objects.h"

/* Some default components and component sets! */

namespace tvx
{

	enum CameraType {
		TVX_PERSPECTIVE_CAMERA,
		TVX_ORTOGRAPHIC_CAMERA,
		TVX_2D_CAMERA,
	};

	class Camera {
	private:
		CameraType m_type;
		glm::vec3 m_scale; //(!) Width, Height and Plane Distance, in this order!!!
		glm::vec2 m_culling; //Minimum and maximum render distance.
		float m_episilon; //Minimum step a photon will make while rendering.

		float m_lens_width;
	public:
		Camera(CameraType _type, glm::vec3 _scale, glm::vec2 _cul, float epsilon):
			m_type(_type), m_scale(_scale), m_culling(_cul), m_episilon(epsilon){}
		~Camera(){}
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

		Type& operator[](EntityUID e)
		{
			return of(e);
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
