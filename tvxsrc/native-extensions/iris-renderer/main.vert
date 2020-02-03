#version 400 core
layout (location=0) in vec3 model_vpos;
void main() {
  gl_Position = vec4(model_vpos, 1.0);
}
