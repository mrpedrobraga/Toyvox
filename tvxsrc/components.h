#pragma once
#include <math.h>
#include <glm/glm.hpp>
#include <string.h>
#include "objects.h"

/* Some default components and component sets! */

namespace tvx
{

	/* Transforms: position, rotation, that kind of thing... */
	struct Transform : Component
	{
		
	};
	struct TransformSet : ComponentSet
	{
	private:
		Transform* cset[MAX_ENTITIES];
	public:
		void set(EntityUID e, Transform value)
		{
			cset[e] = &value;
		}

		Transform at(EntityUID e)
		{
			return *(cset[e]);
		}

		void apply(void (*f)(Transform))
		{
			for(const Transform* c : cset)
				(*f)(*c);
		}
	};

	/* Type tags, in case you want to differenciate types of entities. */
	struct TypeTag : Component
	{
	private:
		string value;
	public:
		string get_value()
		{
			return value;
		}

		void set_value(string new_value)
		{
			value = new_value;
		}
	};
	struct TypeTagSet : ComponentSet
	{
	private:
		TypeTag* cset[MAX_ENTITIES];
	public:
		void set(EntityUID e, TypeTag value)
		{
			cset[e] = &value;
		}

		TypeTag get(EntityUID e)
		{
			return *(cset[e]);
		}

		void apply(void (*f)(TypeTag))
		{
			for(const TypeTag* c : cset)
				(*f)(*c);
		}
	};
}