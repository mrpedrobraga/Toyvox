#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>

#include <iostream>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <chrono>
#include <thread>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef WIN32
# define WINDOWS_THREADS //If you're using windows threads
# ifdef WINDOWS_THREADS
#   include <windows.h>
#   define sleep(n) Sleep(n)
# else
#   define sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n))
# endif
#endif
