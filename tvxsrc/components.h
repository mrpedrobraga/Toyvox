#pragma once
#include <math.h>
#include "objects.h"
#include <string.h>

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

		void apply(void (*tick)(Type))
		{
			for(const Type c : cset)
				(*tick)(c);
		}

		ComponentSet()
		{
			fill_n(cset, MAX_ENTITIES, Type());
		}
	};
}