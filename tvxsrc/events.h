#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>

class KeyMap {
private:
	std::unordered_map<std::string, SDL_Keycode*> mapped_keys;
public:
	void add(std::string key_name, SDL_Keycode* keys) {
		mapped_keys["name"] = keys;
	}

	bool is(SDL_Keycode& k, std::string key_name) {
		if (std::find(std::begin(mapped_keys), std::end(mapped_keys), k) != std::end(mapped_keys)) return true;
		return false;
	}
}