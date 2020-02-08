#version 420

uniform sampler2D tex;
layout (location = 0) in vec2 resIn;
layout (location = 1) in float timeIn;
layout (location = 2) in float dtIn;
layout (location = 3) in vec4 camPosIn;
layout (location = 4) in vec4 camRotIn;
layout (location = 5) in vec4 controlsIn;
layout (location = 0) out vec4 fsOut;

void main() {
	vec2 coords = gl_FragCoord.xy / controlsIn.xy;
	fsOut = texture(tex, coords);
	fsOut.rgb = pow(fsOut.rgb, vec3(1./2.2));
}
