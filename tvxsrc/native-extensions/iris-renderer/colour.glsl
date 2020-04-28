#version 400 core

/* Time since program has started! */
uniform float time;

/* The screen resolution in pixels. 1/pixelsize */
uniform ivec2 resolution;

layout(origin_upper_left) in vec4 gl_FragCoord;
layout(location = 0) out vec4 finalColor;

/* Camera rotation matrix.
The camera can't be translated nor scaled,
so this is a rotation-only matrix. */
uniform mat4 camera_rotation;

/* Minimum photon step and maximum raymarching steps for the raymarching algorithm */
const float MIN_PHOTON_STEP = 0.01;
const int MAX_RM_STEPS = 1000;

const float multiplier = 10.;

/* The threshold value to give up penetration on translucid voxels. */
const float ALPHA_OPAQUE = 0.99;

//The model to be rendered, passed a group of uniforms and a texture.
  const uint WORLD_SIZE = 1331;
  uniform sampler1D voxels;

//The size of the model, in voxels.
uniform float model_size;

/* The top-northwest of the model. */
vec3 a = vec3(0.5, -.15, -.15);

/* The size of the model */
vec3 b = vec3(.3, .3, .3);

//Function definitions
vec3 raymarch (vec2 uv, vec3 camera_scale);
float ssdf (vec3 p);
vec3 vecFloor (vec3 inv);
vec3 ssnormal (vec3 p);
vec4 getColor (uint color);
vec4 currentColor(vec3 ray);

//#define RAYTRACING

void stepCM (inout vec3 p, in vec3 direction, in float model_size){

    //First determine the cell this point is in.
    //A pair of vectors (minp and maxp)

    float inv_ms = 1./model_size;

    vec3 minp = floor(p * model_size) * inv_ms;
    vec3 maxp = minp + inv_ms;

    vec3 dist = vec3(	direction.x > 0. ? maxp.x - p.x : minp.x - p.x,
                     	direction.y > 0. ? maxp.y - p.y : minp.y - p.y,
                     	direction.z > 0. ? maxp.z - p.z : minp.z - p.z
                     );

    if (abs(direction.z / direction.x) > abs(dist.z / dist.x))
        p += dist.z * vec3(direction.x / direction.z, direction.y / direction.z, 1.);
    else if (abs(direction.y / direction.x) > abs(dist.y / dist.x))
        p += dist.y * vec3(direction.x / direction.y, 1., direction.z / direction.y);
    else
        p += dist.x * vec3(1., direction.y / direction.x, direction.z / direction.x);

    p += 0.001 * direction;
}

//The entry point.
void main ()
{
  vec2 uv = gl_FragCoord.xy / vec2(160, 90);

  vec3 col = raymarch(uv - .5, vec3(1.6, 0.9, 1.0));

  finalColor = vec4(col, 1.0);
}

/* Where all raymarching magic happens. Use the octree to accelerate a pyramid of photons generated here,
and intersect with the octree to get the colour of each screen pixel. */
vec3 raymarch (vec2 uv, vec3 camera_scale)
{
  vec3 ray = (vec4(normalize(vec3(1.0, camera_scale.xy * uv)),1.0)).xyz;
  float dist = 0.;
  vec3 cray = vec3(0.); vec3 tcray;

  for(int steps = 0; steps < MAX_RM_STEPS; steps++)
  {
    #ifdef RAYTRACING
        dist += MIN_PHOTON_STEP; cray = dist * ray;
    #else
        stepCM(cray, ray, float(model_size) * multiplier); //cray += MIN_PHOTON_STEP * ray;
    #endif

    tcray = cray;
    bool outside = tcray.x < a.x || tcray.y < a.y || tcray.z < a.z || tcray.x > a.x+b.x || tcray.y > a.y+b.y || tcray.z > a.z+b.z;

    if (outside) continue;
    vec4 ccol = currentColor(tcray);

    if (ccol.a == 0) continue;

    return ccol.xyz;
  }

  return vec3(1.0);
}

//Linear interpolation
float lerp(float a, float b, float p) {
	return a + p * (b-a);
}

//Linear anti-interpolation
float unlerp(float a, float b, float m) {
	return (m-a)/(b-a);
}

//Get the voxel on the model given the position of the photon, in vx, in world-space coordinates.
// (!) @ deprecated
/* Get the current color of a color cube and completely ignores the model. */
vec4 currentColor (vec3 ray) {

    //vec3 r = ray;
    vec3 r = ( ray - (a + 0.1 * sin(time)) ) / b;

    float index = float(floor(r.x * model_size) + (model_size) * floor(r.y * model_size) + (model_size * model_size) * floor(r.z * model_size));

    return texture(voxels, index);
}

/* Extract the 8-bit RGBA colour from an int using bit magic :) */
vec4 getColor(uint color)
{
  //return vec4(0.0, 0.7, 0.5, 1.0);
  return vec4((color & 0x000000FFu) >> 0, (color & 0x0000FF00u) >> 8, (color & 0x00FF0000u) >> 16, (color & 0xFF000000u)>> 24) / 255.0;
}
