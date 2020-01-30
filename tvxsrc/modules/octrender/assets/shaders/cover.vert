
#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 0) out vec2 resOut;
layout (location = 1) out float timeOut;
layout (location = 2) out float dtOut;

layout (std140, binding = 1) uniform global {
  vec4 pack;
};

void main() {
  resOut = vec2(pack.x, pack.y);
  timeOut = pack.z;
  dtOut = pack.w;
  gl_Position = vec4(inPos, 1.0);
}