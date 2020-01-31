
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

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_log.h>

#include <glad/glad.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>