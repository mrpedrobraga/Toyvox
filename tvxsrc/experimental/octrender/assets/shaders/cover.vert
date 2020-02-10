
#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 0) out vec2 resOut;
layout (location = 1) out float timeOut;
layout (location = 2) out float dtOut;
layout (location = 3) out vec4 camPosOut;
layout (location = 4) out vec4 camRotOut;
layout (location = 5) out vec4 controlsOut;

layout (std140, binding = 0) uniform global {
  vec4 pack;
  vec4 camPos;
  vec4 camRot;
  vec4 controls;
};

void main() {
  resOut = vec2(pack.x, pack.y);
  timeOut = pack.z;
  dtOut = pack.w;
  camPosOut = camPos;
  camRotOut = camRot;
  controlsOut = controls;
  gl_Position = vec4(inPos, 1.0);
}