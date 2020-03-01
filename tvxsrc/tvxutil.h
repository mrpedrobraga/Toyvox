#pragma once

#include "tvxoptions.h"

#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>

#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <chrono>
#include <thread>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <bitset>
#include <string>
#include <vector>
#include <chrono>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void errorlog(const char* x) {printf(x);}

/* Available renderers */

using Voxel = uint32_t;
using SparseData = std::bitset<8>;
using UserData = std::bitset<10>;
using Colour = std::bitset<32>;
using Normal = std::bitset<24>;

#ifdef WIN32
# define WINDOWS_THREADS //If you're using windows threads
# ifdef WINDOWS_THREADS
#   include <windows.h>
#   define sleep(n) Sleep(n)
# else
#   define sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n))
# endif
#endif

std::chrono::steady_clock::time_point program_beginning = std::chrono::steady_clock::now();

auto wall_clock_time(){
  return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - program_beginning).count();
}
