#version 400 core

#define RAYTRACING

uniform float time;
uniform vec2 resolution;

//layout(origin_upper_left) in vec4 gl_FragCoord;

/* Default is pointing X+ in rectangular coordinates.

Increasing X rotates the screen clockwise,
  Y rotates the screen upwards,
    Z rotates the screen to the right.*/
vec3 camera_rotation = vec3(0.0, radians(30.), 0.0);

const float min_step = 0.001;
const int max_stepc = 500;

int ccount = 8;
vec4 palette[8] = const vec4[8](
  vec4(1.0, 0.0, 0.0, 1.0),
    vec4(1.0, 0.1, 0.1, 1.0),
    vec4(0.9, 0.3, 0.3, 1.0),
    vec4(0.7, 0.0, 0.3, 1.0),
    vec4(1.0, 0.0, 1.0, 1.0),
    vec4(1.0, 1.0, 0.0, 1.0),
    vec4(0.0, 1.0, 1.0, 1.0),
    vec4(0.0, 0.0, 0.0, 1.0)
);

vec3 model_size = vec3(4);

/**/const int voxels[64] = int[64](

 0, 0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, 0, 0, 0, -1, -1, -1, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, -1, -1, 0, -1, 0, 0, 0, 0, -1, -1, 0, -1, -1, -1, 0, -1, 0, 0, 0, -1

 );/**/

vec3 a = vec3(.12, .0, -.1);
vec3 b = vec3(.05, .05, .05);

vec3 raymarch (vec2 uv, vec3 camera_scale);
float ssdf (vec3 p);
vec3 vecFloor (vec3 inv);
vec3 ssnormal (vec3 p);
vec3 rotate3d(vec3 vector, vec3 rotations);
int currentVoxel(vec3 ray);

void main ()
{
  vec2 uv = gl_FragCoord.xy / vec2(640., 360.);
   gl_FragCoord.xy;
  //camera_rotation.z += mouseuv.x;
  //camera_rotation.y -= mouseuv.y * 1.5;

  vec3 col = raymarch(uv - .5, vec3(1.6, 0.9, 1.0));

  gl_FragColor = vec4(col, 1.0);
}

vec3 raymarch (vec2 uv, vec3 camera_scale)
{
  vec3 ray = rotate3d(normalize(vec3(1.0, camera_scale.xy * uv)), camera_rotation);
  float dist = 0.;
  vec3 cray = vec3(0.);

  for(int steps = 0; steps < max_stepc; steps++)
  {

    dist += min_step;

    cray = (dist * ray);

    int cv = currentVoxel(cray);

    if(cv != -1)
    {
      if(cv >= ccount || cv < 0) continue;

      vec4 col = palette[cv];
      return mix(col.rgb, vec3(1.0), 1.0 - col.a);
    }
  }

  return vec3(1.0);
}

float lerp(float a, float b, float p) {
	return a + p * (b-a);
}

float unlerp(float a, float b, float m) {
	return (m-a)/(b-a);
}

int currentVoxel(vec3 ray) {

    bool outside = ray.x < a.x || ray.y < a.y || ray.z < a.z || ray.x > a.x+b.x || ray.y > a.y+b.y || ray.z > a.z+b.z;

    if(outside) return -1;

    vec3 r = ( ray - a ) / b;

    return voxels[int(
        				floor(r.x * model_size.x) + 4.0 * floor(r.y * model_size.y) + 16.0 * floor(r.z * model_size.z)
    				 )
                 ];
}

float ssdf (vec3 p) {

  vec4 object = vec4(-2.0, -2.0, -2.0, 2.0);

  float cd = distance(p.x, object.x) - object.w/2.;
        cd = max(cd, distance(p.y, object.y) - object.w/2.);
        cd = max(cd, distance(p.z, object.z) - object.w/2.);

  return (length(object.xyz - p) - object.w/2.);
  //return max(cd, -(length(object.xyz - p) - object.w/2. * 1.2));
}

vec3 ssnormal(vec3 p) {
    return normalize(vec3(
        ssdf(vec3(p.x + min_step, p.y, p.z)) - ssdf(vec3(p.x - min_step, p.y, p.z)),
        ssdf(vec3(p.x, p.y + min_step, p.z)) - ssdf(vec3(p.x, p.y - min_step, p.z)),
        ssdf(vec3(p.x, p.y, p.z  + min_step)) - ssdf(vec3(p.x, p.y, p.z - min_step))
    ));
}

highp vec3 vecFloor(vec3 inv) {
  return inv;
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 rotate3d(vec3 vector, vec3 rotations) {

  mat4 rx = rotationMatrix(vec3(1.0, 0.0, 0.0), rotations.x);
  mat4 ry = rotationMatrix(vec3(0.0, -1.0, 0.0), rotations.y);
  mat4 rz = rotationMatrix(vec3(0.0, 0.0, -1.0), rotations.z);

  return (rz * ry * rx * vec4(vector, 1.0)).xyz;
}
