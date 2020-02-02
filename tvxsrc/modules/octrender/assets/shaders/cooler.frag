#version 420 core




layout (location = 0) in vec2 resIn;
layout (location = 1) in float timeIn;
layout (location = 2) in float dtIn;
layout (location = 3) in vec4 camPosIn;
layout (location = 4) in vec4 camRotIn;
layout (location = 5) in vec4 controlsIn;
layout (location = 0) out vec4 fsOut;
layout (binding = 0) uniform sampler2D dataTex;


#define debrisdetail 10
#define steps 1000
#define ambient 0.2

#define emptycells 0.4
#define subdivisions 0.90//should be higher than emptycells

#define detaildata 5
#define datapixels 6

#define cube_nothing 0
#define cube_octree 1
#define cube_empty 2
#define cube_brick 3

#define epsilon max(0.001*exp(-float(detail)), 0.0000001)


float rnd(vec4 v) { return fract(4e4*sin(dot(v, vec4(13.46, 41.74, -73.36, 14.24))+17.34)); }

//0 is empty, 1 is subdivide and 2 is full
int getvoxel(vec3 p, float size) {
	#ifdef objects
	if (p.x==0.0&&p.y==0.0) {
		return 0;
	}
		#endif
	
	float val = rnd(vec4((p/size), size));
	
	if (val < emptycells) {
		return cube_empty;
	} else if (val < subdivisions) {
		return cube_octree;
	} else {
		return cube_brick;
	}
}

vec4 getdata(int index) {
	ivec2 p;
	p.x = index%int(resIn.x);
	p.y = index/int(resIn.x);
	return texelFetch(dataTex, p, 0);
}

vec3 voxel(vec3 ro, vec3 rd, float size) {
	size *= 0.5;
	
	vec3 hit = -(sign(rd)*(ro-size)-size)/max(abs(rd), 0.001);
	
	return hit;
}

float map(vec3 p) {
	p = mod(p, 0.1)-0.05;
	return length(p)-0.02;
}

vec3 findnormal(vec3 p) {
	vec2 e = vec2(0.0, 0.001);
	
	return normalize(vec3(map(p+e.yxx), map(p+e.xyx), map(p+e.xxy))-map(p));
}

vec4 octreeray(vec3 ro, vec3 rd, float maxdist, float e, inout float edge) {
	edge = 1.0;
	
	float size = 0.5;
	vec3 lro = mod(ro, size);
	vec3 fro = ro-lro;
	vec3 mask = vec3(0);
	vec3 lastmask = vec3(0);
	bool exitoct = false;
	int recursions = 0;
	int recursions0 = 0;
	int recursions1 = 0;
	int voxelstate1;
	float dist = 0.0;
	int i;
	int index = 0;
	vec4 data;
	vec3 invrd = 1.0/abs(rd);
	vec3 hit = voxel(lro, rd, size);
	
	if (any(greaterThan(abs(ro-0.5), vec3(0.5)))) return vec4(0);
	
	//the octree traverser loop
	//each iteration i:
	// - check if i need to go up a level
	// - check if i need to go down a level
	// - check if i hit a cube
	// - go one step forward if cube is empty
	for (i = 0; i < steps; i++)
	{
		if (dist > maxdist) break;
		
		if (recursions0 == recursions) {
			vec3 q = mod(floor(fro/size+0.5)+0.5, 2.0)-0.5;
			data = getdata(index*8+datapixels+int(dot(q, vec3(1, 2, 4))+0.5));
		}
		int voxelstate = int(data.w);
		
		if (recursions1 == recursions) {
			voxelstate1 = getvoxel(fro, size);
			if (recursions1 == debrisdetail-1 && voxelstate1 == cube_octree) {
				voxelstate1 = cube_empty;
			}
		}
		
		bool isnothing = recursions0 < recursions || voxelstate == cube_nothing;
		
		if (isnothing) {
			
			voxelstate = voxelstate1;
		}
		
		//i go up a level
		if (exitoct)
		{
			
			if (recursions0 == recursions) {
				index = int(data.x);
				recursions0--;
			}
			
			if (recursions1 == recursions) {
				recursions1--;
			}
			
			vec3 newfro = floor(fro/size*0.5+0.25)*size*2.0;
			
			lro += fro-newfro;
			fro = newfro;
			
			recursions--;
			size *= 2.0;
			hit = voxel(lro, rd, size);
			if (recursions < 0) break;
			exitoct = (abs(dot(mod(fro/size+0.5, 2.0)-1.0+mask*sign(rd)*0.5, mask))<0.1);
		}
		//subdivide
		else if (voxelstate == cube_octree)
		{
			recursions++;
			
			if (!isnothing) {
				index = int(data.y);
				recursions0++;
			}
			
			if (voxelstate1 == cube_octree) {
				recursions1++;
			}
			
			size *= 0.5;
			
			//find which of the 8 voxels i will enter
			vec3 mask2 = step(vec3(size), lro);
			fro += mask2*size;
			lro -= mask2*size;
			hit = voxel(lro, rd, size);
		}
		//move forward
		else if (voxelstate == cube_nothing || voxelstate == cube_empty)
		{
			//raycast and find distance to nearest voxel surface in ray direction
			if (hit.x < min(hit.y, hit.z)) {
				mask = vec3(1, 0, 0);
			} else if (hit.y < hit.z) {
				mask = vec3(0, 1, 0);
			} else {
				mask = vec3(0, 0, 1);
			}
			float len = dot(hit, mask);
			
			hit -= len;
			
			hit += mask*invrd*size;
			
			lro += rd*len-mask*sign(rd)*size;
			vec3 newfro = fro+mask*sign(rd)*size;
			
			#ifdef raymarching
			if (voxelstate == cube_nothing) {
				if (map(ro+dist*rd) < e) {
					mask = lastmask;
					break;
				}
				int j = 100;
				while (len > 0.0 && j > 0) {
					j--;
					float l = map(ro+dist*rd);
					if (l < e) {
						return vec4(dist, vec3(0));
					}
					dist += l;
					len -= l;
				}
			}
				#endif
			
			dist += len;
			exitoct = (floor(newfro/size*0.5+0.25)!=floor(fro/size*0.5+0.25));
			fro = newfro;
			lastmask = mask;
		}
		else
		{
			break;
		}
		if (controlsIn.z > 0.0) { // draw grid
			vec3 q = abs(lro/size-0.5)*(1.0-lastmask);
			edge = min(edge, -(max(max(q.x, q.y), q.z)-0.5)*2000.0*size);
		}
	}
	return vec4(dist, -mask*sign(rd));
}

void main()
{
	int detail = int(getdata(detaildata).x);
	
	vec2 uv = (gl_FragCoord.xy * 2.0 - resIn) / resIn.y;
	
	vec3 rd = normalize(vec3(uv, 1));
	rd.zy *= mat2(cos(camRotIn.y), -sin(camRotIn.y), sin(camRotIn.y), cos(camRotIn.y));
	rd.zx *= mat2(cos(camRotIn.x), -sin(camRotIn.x), sin(camRotIn.x), cos(camRotIn.x));
	
	float edge = 1.0;
	vec4 len = octreeray(camPosIn.xyz, rd, 4.0, epsilon, edge);
	vec3 ro = camPosIn.xyz+rd*len.x;
	
	vec3 p1 = vec3(0);
	if (controlsIn.z < 2) {
		p1 = ro*exp2(float(detail)) * edge;
	} else {
		p1 = vec3(edge);
	}
	
	bool voxel = len.yzw != vec3(0);
	
	if (!voxel) {
		len.yzw = findnormal(ro);
	}
	
	fsOut.xyz = vec3(1.0, 0.6, 0.4);
	fsOut *= fsOut;
	
	if (controlsIn.z > 1.0) {
		float a = mod(dot(floor(p1), vec3(1)), 2.0);
		fsOut = fsOut * a;
	}
	
	p1 = abs(fract(p1) - 0.5);
	if (voxel) {
		p1 = min(p1, p1.yzx);
	}
	float b = (1.0-max(max(p1.x, p1.y), p1.z)*2.0/(len.x*exp2(float(detail))*0.04+1.0));
	fsOut = fsOut * b;
	
	vec3 normal = len.yzw;
	vec3 lightpos = vec3(0.5, 0.3, 0.5);
	vec3 lightdir = ro-lightpos;
	float lightdist = length(lightdir);
	lightdir /= lightdist;
//	lightdir *= 4.0;
	//	float shadow = float(octreeray(lightpos, lightdir, lightdist, epsilon).x>lightdist-epsilon);
	//	fsOut *= max(dot(-lightdir, normal)*shadow, ambient);
	fsOut *= max(dot(-lightdir, normal), ambient);
	
	fsOut = sqrt(fsOut);
	fsOut.rgb = pow(fsOut.rgb, vec3(1./2.2));
}