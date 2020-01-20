#pragma once
#include <math.h>
#include <glm/glm.hpp>
#include <string.h>
#include "objects.h"

/* Some default components and component sets! */

namespace tvx
{

	/* Type tags, in case you want to differenciate types of entities. */
	struct TypeTag
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

		TypeTag()
		{
			value = "";
		}

		TypeTag(string new_value)
		{
			value = new_value;
		}
	};

	template <class Type>
	struct ComponentSet
	{
	private:
		Type cset[MAX_ENTITIES];
	public:
		void set(EntityUID e, Type value)
		{
			cset[e] = value;
		}

		Type get(EntityUID e)
		{
			return (cset[e]);
		}

		void apply(void (*f)(Type))
		{
			for(const Type c : cset)
				(*f)(c);
		}

		ComponentSet()
		{
			fill_n(cset, MAX_ENTITIES, Type());
		}
	};
}