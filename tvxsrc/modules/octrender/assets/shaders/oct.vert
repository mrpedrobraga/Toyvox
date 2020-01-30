
#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 uniforms;

layout (std140, binding = 1) uniform u {
  vec4 vars;
};

void main() {
  outPos = vec4(inPos, 1.0);
  uniforms = vars;
}