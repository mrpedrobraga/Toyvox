#pragma once

#include "options.h"
#include "tvxutil.h"

namespace tvx {

	class EventSheet
	{

	};

	/**
	 * An associative container linking key presses to descriptive action strings
	 */
	class KeyMap
	{
		private:
			std::multimap<std::string, SDL_Keycode> action_to_key;
			/*
				(!) Under reconstruction:
				Multiple keys can point to the same action, one key can
				point to multiple actions.

				The sections under reconstruction will be implemented after
				everything is working like a breeze.
			std::unordered_multimap<SDL_Keycode, std::string> key_to_action;*/
		public:

			/**
			 * Associate an action with a keypress
			 * @param action
			 * @param key
			 */
			void add(const std::string& action, SDL_Keycode key) {
				action_to_key.insert(std::pair<std::string, SDL_Keycode>(action, key));
				//(!) key_to_action.insert(std::pair<SDL_Keycode, std::string>(key, action));
			}

			/** (!) Under reconstruction
			 * Get the action that is associated with a given key
			 * @param key
			 * @return associated action if it exists, empty string if not
			 *
			std::string get_action(SDL_Keycode key) {
				return key_to_action.count(key) ? key_to_action[key] : "";
			}*/

			/**
			 * Checks if a given key triggers a certain action!
			 * @param key
			 * @param action
			 * @return true if the given key triggers the action, false otherwise
			 */
			bool is_action(SDL_Keycode key, std::string& action) {

				auto itr_start = action_to_key.lower_bound(action);
			    auto itr_end = action_to_key.upper_bound(action);

			    while (itr_start != itr_end)
			    {
			        if (itr_start -> first == action && itr_start -> second == key) return true;
			        itr_start++;
			    }

        		return false;
			}

			bool is_action(SDL_Keycode key, const std::string& action) {

				auto itr_start = action_to_key.lower_bound(action);
			    auto itr_end = action_to_key.upper_bound(action);

			    while (itr_start != itr_end)
			    {
			        if (itr_start -> first == action && itr_start -> second == key) return true;
			        itr_start++;
			    }

        		return false;
			}


			/** (!) Under reconstruction
 			 * Get the key that as associated with a given action
			 * @param action
			 * @return associated key if it exists, SDLK_UNKNOWN if not

			SDL_Keycode get_key(const std::string& action) {
				return action_to_key.count(action) ? action_to_key[action] : SDLK_UNKNOWN;
			}*/

	};
}
