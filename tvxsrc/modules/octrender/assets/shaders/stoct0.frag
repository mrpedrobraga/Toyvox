#version 420 core




layout (location = 0) in vec2 resIn;
layout (location = 1) in float timeIn;
layout (location = 2) in float dtIn;
layout (location = 3) in vec4 camPosIn;
layout (location = 4) in vec4 camRotIn;
layout (location = 0) out vec4 fsOut;
layout (binding = 0) uniform sampler2D dataTex;


//uncomment to add some spheres you can mess with
#define raymarching

//the editor starts with octree lvl 8 (or startdetail) when you refresh
#define startdetail 8
//limit octree level to 20 (or maxdetail) and 1 (or mindetail)
#define maxdetail 20
#define mindetail 1
#define debrisdetail 20
//octree iterations before the ray gives up, gets really goofy on low numbers
#define steps 1000
#define ambient 0.2

#define emptycells 0.5
#define subdivisions 0.90//should be higher than emptycells


#define counter 0
#define lastmouse 1
#define campos 2
#define camrot 3
#define clickray 4
#define detaildata 5
#define datapixels 6

#define cube_nothing 0
#define cube_octree 1
#define cube_empty 2
#define cube_brick 3

#define epsilon max(0.001*exp(-float(detail)), 0.0000001)

#define sensitivity 3.0
#define movespeed (3.0*exp2(float(-detail)))

#define rot(A) mat2(cos(A), -sin(A), sin(A), cos(A))

//random function from https://www.shadertoy.com/view/MlsXDf
float rnd(vec4 v) { return fract(4e4*sin(dot(v, vec4(13.46, 41.74, -73.36, 14.24))+17.34)); }
	
	//hash function by Dave_Hoskins https://www.shadertoy.com/view/4djSRW
	#define HASHSCALE3 vec3(.1031, .1030, .0973)
vec3 hash33(vec3 p3)
{
	p3 = fract(p3 * HASHSCALE3);
	p3 += dot(p3, p3.yxz+19.19);
	return fract((p3.xxy + p3.yxx)*p3.zyx);
}

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
	
	
	//keyboard keys
	#define KEY_UP    38
	#define KEY_DOWN  40
	#define KEY_RIGHT 39
	#define KEY_LEFT  37
	#define KEY_SHIFT 16
	#define KEY_CTRL  17
	#define KEY_Q  81
	#define KEY_E  69
	#define KEY_O  79
	#define KEY_L  76
	
	//movement keys
	#define MOVE_FORW   KEY_UP
	#define MOVE_LEFT   KEY_LEFT
	#define MOVE_RIGHT  KEY_RIGHT
	#define MOVE_BACK   KEY_DOWN
	#define MOVE_UP     KEY_SHIFT
	#define MOVE_DOWN   KEY_CTRL
	#define ERASE       KEY_E
	#define UP_DETAIL   KEY_O
	#define DOWN_DETAIL KEY_L

float getkey(int x, int y)
{
	return texelFetch(dataTex, ivec2(x, y), 0).x;
}

vec4 getdata(int index) {
	ivec2 p;
	p.x = index%int(resIn.x);
	p.y = index/int(resIn.x);
	return texelFetch(dataTex, p, 0);
}

float map(vec3 p) {
	p = mod(p, 0.1)-0.05;
	return length(p)-0.02;
}

vec3 voxel(vec3 ro, vec3 rd, float size)
{
	size *= 0.5;
	
	vec3 hit = -(sign(rd)*(ro-size)-size)/max(abs(rd), 0.0001);
	
	return hit;
}

vec4 octreeray(vec3 ro, vec3 rd, float maxdist, float e) {
	
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
		}
		
		bool isnothing = recursions0 < recursions || voxelstate == cube_nothing;
		
		if (isnothing) {
			voxelstate = voxelstate1;
		}
		
		if (recursions == maxdetail-1 && voxelstate == cube_octree) {
			voxelstate = cube_empty;
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
	}
	return vec4(dist, -mask*sign(rd));
}

vec4 updatedata(int index) {
	vec4 data = getdata(index);
	
	int detail = int(getdata(detaildata).x);
	
	vec2 ndcPointAt = vec2(0);
	
	vec4 cray = getdata(clickray);
	
	vec4 iMouse2 = getdata(lastmouse);
	//bool click = cray.w > 0.5;
	bool click = iMouse2.z > 0.0 && /*iMouse.z < 0.0*/false && length(iMouse2.xy-iMouse2.zw)<10.0;
	bool erase = getkey(ERASE, 0) > 0.5;
	
	vec3 mpos = cray.xyz;
	
	if (any(greaterThan(abs(mpos-0.5), vec3(0.5)))) click = false;
	
	vec4 counterdata = getdata(counter);
	
	bool refresh = (false)// TODO, or scrap?
	||(resIn.xy!=counterdata.zw);
	
	
	float size = 0.5;
	vec4 data2;
	int index3 = 0;
	vec3 p;
	int i = 0;
	while (true) {
		p = mod(floor(mpos/size)+0.5, 2.0)-0.5;
		data2 = getdata(index3*8+datapixels+int(dot(p, vec3(1, 2, 4))));
		if (int(data2.w) != cube_octree||i == detail-1) {
			break;
		}
		i++;
		size /= 2.0;
		index3 = int(data2.y);
	}
	int subdivides = detail-i-1;
	
	
	if (index == counter) {
		if (refresh) {
			data = vec4(0, 0, resIn.xy);
			return data;
		}
		if (click) {
			data.x += float(max(subdivides, 0));
		}
		return data;
	} else if (index == campos) {
		return camPosIn;		
	} else if (index == camrot) {
		return camRotIn;
	} else if (index == clickray) {
		
		if (refresh) {
			return vec4(0);
		}
		if (/*iMouse.z > 0.0*/false && iMouse2.z < 0.0) {
			vec4 rotcam = getdata(camrot);
			
			vec3 ro = getdata(campos).xyz;
			vec3 rd = normalize(vec3(ndcPointAt, 1));
			rd.zy *= mat2(cos(rotcam.y), -sin(rotcam.y), sin(rotcam.y), cos(rotcam.y));
			rd.zx *= mat2(cos(rotcam.x), -sin(rotcam.x), sin(rotcam.x), cos(rotcam.x));
			vec4 len = octreeray(ro, rd, 1.0, epsilon);
			vec3  normal = len.yzw;
			float minsize = 0.5/exp2(float(detail));
			
			if (erase) {
				ro += len.x*rd-normal*epsilon;
			} else {
				ro += len.x*rd+normal*epsilon;
			}
			
			ro = clamp(ro, epsilon, 1.0-epsilon);
			
			data.xyz = floor(ro/minsize)*minsize;
			//data.w = float((getkey(KEY_E,1)>0.5)||(getkey(KEY_Q,1)>0.5));
		}
		return data;
		
	} else if (index == detaildata) {
		
		if (refresh) {
			return vec4(startdetail);
		}
		
		float keys = getkey(UP_DETAIL, 1)-getkey(DOWN_DETAIL, 1);
		
		data.x = clamp(data.x+keys, float(mindetail), float(maxdetail));
		
		data.y = keys;
		
		return data;
	} else {
		
		if (refresh) {
			data = vec4(-1, -1, 0, 0);
			return data;
		}
		if (click) {
			int index2 = index-datapixels;
			int quad = index2/8;
			int quadindex = index2-quad*8;
			if (quad <= int(counterdata.x)) {
				//if (data.y < -0.5)
				{
					if (index3 == quad) {
						vec3 clickp = mod(floor(mpos/size)+0.5, 2.0)-0.5;
						int clicki = quad*8+int(dot(clickp, vec3(1, 2, 4)));
						
						if (clicki == index2) {
							if (i == detail-1) {
								if (erase) {
									data.w = float(cube_empty);
								} else {
									data.w = float(cube_brick);
								}
								data.y = -1.0;
								return data;
							}
							data.w = float(cube_octree);
							data.y = counterdata.x+1.0;
						}
					}
				}
			} else if (quad <= int(counterdata.x)+subdivides) {
				vec3 q = mod(floor(mpos*exp2(float(i+quad)-counterdata.x+1.0))+0.5, 2.0)-0.5;
				vec3 q2 = vec3(quadindex%2, (quadindex/2)%2, quadindex/4);
				
				if (quad == int(counterdata.x)+1) {
					data.x = float(index3);
				} else {
					data.x = float(quad-1);
				}
				
				if (dot(abs(q-q2), vec3(1))<0.1) {
					data.w = float(cube_octree);
					data.y = float(quad+1);
					if (quad == int(counterdata.x)+subdivides) {
						if (erase) {
							data.w = float(cube_empty);
						} else {
							data.w = float(cube_brick);
						}
						data.y = -1.0;
					}
				} else {
					data.w = data2.w;
					data.y = -1.0;
				}
			}
		}
	}
	
	
	return data;
}

void main()
{
	ivec2 coord = ivec2(gl_FragCoord);
	fsOut = updatedata(coord.x+coord.y*int(resIn.x));
}