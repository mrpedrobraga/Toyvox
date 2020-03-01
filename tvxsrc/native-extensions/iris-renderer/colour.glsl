#version 400 core

/* Time since program has started! */
uniform float time;

/* The screen resolution in pixels. 1/pixelsize */
uniform ivec2 resolution;

layout(origin_upper_left) in vec4 gl_FragCoord;

/* Camera rotation matrix.
The camera can't be translated nor scaled,
so this is a rotation-only matrix. */
uniform mat4 camera_rotation;

/* Minimum photon step in raymarching algorithm */
const float MIN_PHOTON_STEP = 0.001;

/* Maximum steps before giving up on rendering. */
const int MAX_RM_STEPS = 500;

/* The threshold value to give up penetration on translucid voxels. */
const float ALPHA_OPAQUE = 0.99;

//The model to be rendered, passed as an uniform.
uniform uint voxels[64];

//The size of the model, in voxels.
uniform float model_size;

//The origin (top_northwest of the octree)
vec3 a = vec3(.3, -.1, -.05);

//The other point of the diagonal (bottom_southeast of the octree)
vec3 b = vec3(.1, .1, .1);

//Function definitions
vec3 raymarch (vec2 uv, vec3 camera_scale);
float ssdf (vec3 p);
vec3 vecFloor (vec3 inv);
vec3 ssnormal (vec3 p);
vec4 getColor (uint color);
int currentVoxel(vec3 ray);

//The entry point.
void main ()
{
  vec2 uv = gl_FragCoord.xy / vec2(640, 360);

  vec3 col = raymarch(uv - .5, vec3(1.6, 0.9, 1.0));

  gl_FragColor = vec4(col, 1.0);
}

/* Where all raymarching magic happens. Use the octree to accelerate a pyramid of photons generated here,
and intersect with the octree to get the colour of each screen pixel. */
vec3 raymarch (vec2 uv, vec3 camera_scale)
{
  vec3 ray = (camera_rotation * vec4(normalize(vec3(1.0, camera_scale.xy * uv)), 1.0)).xyz;
  float dist = 0.;
  vec3 cray = vec3(0.);

  for(int steps = 0; steps < MAX_RM_STEPS; steps++)
  {
    dist += MIN_PHOTON_STEP;
    cray = (dist * ray);

    int cv = currentVoxel(cray);
    if (cv == -1) continue;
    vec4 col = getColor(uint(cv));

    if (col.a == 0) continue;

    return col;
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
int currentVoxel(vec3 ray)
{
    bool outside = ray.x < a.x || ray.y < a.y || ray.z < a.z || ray.x > a.x+b.x || ray.y > a.y+b.y || ray.z > a.z+b.z;

    if(outside) return -1;

    vec3 r = ( ray - a ) / b;

    return int(voxels[int(floor(r.x * model_size) + 4.0 * floor(r.y * model_size) + 16.0 * floor(r.z * model_size))]);
}

/* Extract the 8-bit RGBA colour from an int using bit magic :) */
vec4 getColor(uint color)
{
  //return vec4(0.0, 0.7, 0.5, 1.0);
  return vec4((color & 0x000000FF) >> 0, (color & 0x0000FF00) >> 8, (color & 0x00FF0000) >> 16, (color & 0xFF000000)>> 24) / 255.0;
}
