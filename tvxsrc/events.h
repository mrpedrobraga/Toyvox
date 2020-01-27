#pragma once
#include "tvxutil.h"

namespace tvx {
	
	/**
	 * An associative container linking key presses to descriptive action strings
	 */
	class KeyMap {
		private:
			std::unordered_map<std::string, SDL_Keycode> action_to_key;
			std::unordered_map<SDL_Keycode, std::string> key_to_action;
		public:
			
			/**
			 * Associate an action with a keypress
			 * @param action 
			 * @param key 
			 */
			void add(const std::string& action, SDL_Keycode key) {
				action_to_key[action] = key;
				key_to_action[key] = action;
			}
			
			/**
			 * Get the action that is associated with a given key
			 * @param key 
			 * @return associated action if it exists, empty string if not
			 */
			std::string getAction(SDL_Keycode key) {
				return key_to_action.count(key) ? key_to_action[key] : "";
			}
			
			/**
			 * Get the key that as associated with a given action
			 * @param action 
			 * @return associated key if it exists, SDLK_UNKNOWN if not
			 */
			SDL_Keycode getKey(const std::string& action) {
				return action_to_key.count(action) ? action_to_key[action] : SDLK_UNKNOWN;
			}
	};
}