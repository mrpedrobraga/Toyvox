//#version 330 core

#define MAX_STEPS 500
#define MIN_STEP 0.01

vec3 camera_position = vec3(0.0, 0.0, 0.0);
vec3 camera_rotation = vec3(0.0, 0.0, 0.0);
vec3 camera_scale = vec3(1.0, 1.6, 0.9);

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

vec4 spheresdf(vec3 p, vec3 scenter, float radius) {

  vec4 r;
  vec3 point = rotate3d(p-scenter, vec3(0.0, 0.0, 0.0))+scenter;

  float cd = abs(point.x-scenter.x)-radius;
  cd = max(cd, abs(point.y-scenter.y)-radius);
  cd = max(cd, abs(point.z-scenter.z)-radius);
  r.x = cd;

  return r;
}

vec4 raymarch(vec2 uv) {

  vec3 origin = camera_position;
  vec3 nray = rotate3d(normalize(vec3(camera_scale.x, camera_scale.y * uv.x, camera_scale.z * uv.y)), camera_rotation);

  float dist; vec3 cray = origin;

  for(int steps = 0; steps < MAX_STEPS; steps++)
  {
    vec4 sdist = spheresdf(cray, vec3(3.0, 0.0, 0.0), 1.0);
    dist += sdist.x;

    cray = dist * nray;

    if(sdist.x < MIN_STEP) {
      vec3 normal = sdist.yzw;
      return vec4(vec3(0.1, 0.4, 0.8), 1.0);
    }
  }

  return vec4(0.0, 0.0, 0.0, 0.0);
}

void main()
{
  vec2 uv = gl_FragCoord.xy / 480.0 - .5;
  uv.y *= 16./9.;

  gl_FragColor = raymarch(uv);
}
