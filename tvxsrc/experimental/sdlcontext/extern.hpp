
#pragma once

#include <cmath>
#include <cassert>
#include <cstdarg>
#include <cstdio>

#include <string>
#include <fstream>
#include <memory>
#include <atomic>
#include <functional>
#include <unordered_map>
#include <deque>
#include <array>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_log.h>
#include <glad/glad.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Useful constants
constexpr double halfPi = M_PI / 2;

// Compile time power function from https://stackoverflow.com/a/27271374
template<typename T>
constexpr T sqr(T a) {
	return a * a;
}
template<typename T>
constexpr T power(T a, std::size_t n) {
	return n == 0 ? 1 : sqr(power(a, n / 2)) * (n % 2 == 0 ?  1 : a);
}
