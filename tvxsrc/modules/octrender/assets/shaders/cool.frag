#version 420 core

const uint mortonX[256] = {
0x000000, 0x000001, 0x000008, 0x000009, 0x000040, 0x000041, 0x000048, 0x000049, 0x000200, 0x000201, 0x000208, 0x000209,
0x000240, 0x000241, 0x000248, 0x000249, 0x001000, 0x001001, 0x001008, 0x001009, 0x001040, 0x001041, 0x001048, 0x001049,
0x001200, 0x001201, 0x001208, 0x001209, 0x001240, 0x001241, 0x001248, 0x001249, 0x008000, 0x008001, 0x008008, 0x008009,
0x008040, 0x008041, 0x008048, 0x008049, 0x008200, 0x008201, 0x008208, 0x008209, 0x008240, 0x008241, 0x008248, 0x008249,
0x009000, 0x009001, 0x009008, 0x009009, 0x009040, 0x009041, 0x009048, 0x009049, 0x009200, 0x009201, 0x009208, 0x009209,
0x009240, 0x009241, 0x009248, 0x009249, 0x040000, 0x040001, 0x040008, 0x040009, 0x040040, 0x040041, 0x040048, 0x040049,
0x040200, 0x040201, 0x040208, 0x040209, 0x040240, 0x040241, 0x040248, 0x040249, 0x041000, 0x041001, 0x041008, 0x041009,
0x041040, 0x041041, 0x041048, 0x041049, 0x041200, 0x041201, 0x041208, 0x041209, 0x041240, 0x041241, 0x041248, 0x041249,
0x048000, 0x048001, 0x048008, 0x048009, 0x048040, 0x048041, 0x048048, 0x048049, 0x048200, 0x048201, 0x048208, 0x048209,
0x048240, 0x048241, 0x048248, 0x048249, 0x049000, 0x049001, 0x049008, 0x049009, 0x049040, 0x049041, 0x049048, 0x049049,
0x049200, 0x049201, 0x049208, 0x049209, 0x049240, 0x049241, 0x049248, 0x049249, 0x200000, 0x200001, 0x200008, 0x200009,
0x200040, 0x200041, 0x200048, 0x200049, 0x200200, 0x200201, 0x200208, 0x200209, 0x200240, 0x200241, 0x200248, 0x200249,
0x201000, 0x201001, 0x201008, 0x201009, 0x201040, 0x201041, 0x201048, 0x201049, 0x201200, 0x201201, 0x201208, 0x201209,
0x201240, 0x201241, 0x201248, 0x201249, 0x208000, 0x208001, 0x208008, 0x208009, 0x208040, 0x208041, 0x208048, 0x208049,
0x208200, 0x208201, 0x208208, 0x208209, 0x208240, 0x208241, 0x208248, 0x208249, 0x209000, 0x209001, 0x209008, 0x209009,
0x209040, 0x209041, 0x209048, 0x209049, 0x209200, 0x209201, 0x209208, 0x209209, 0x209240, 0x209241, 0x209248, 0x209249,
0x240000, 0x240001, 0x240008, 0x240009, 0x240040, 0x240041, 0x240048, 0x240049, 0x240200, 0x240201, 0x240208, 0x240209,
0x240240, 0x240241, 0x240248, 0x240249, 0x241000, 0x241001, 0x241008, 0x241009, 0x241040, 0x241041, 0x241048, 0x241049,
0x241200, 0x241201, 0x241208, 0x241209, 0x241240, 0x241241, 0x241248, 0x241249, 0x248000, 0x248001, 0x248008, 0x248009,
0x248040, 0x248041, 0x248048, 0x248049, 0x248200, 0x248201, 0x248208, 0x248209, 0x248240, 0x248241, 0x248248, 0x248249,
0x249000, 0x249001, 0x249008, 0x249009, 0x249040, 0x249041, 0x249048, 0x249049, 0x249200, 0x249201, 0x249208, 0x249209,
0x249240, 0x249241, 0x249248, 0x249249};
const uint mortonY[256] = {
0x000000, 0x000002, 0x000010, 0x000012, 0x000080, 0x000082, 0x000090, 0x000092, 0x000400, 0x000402, 0x000410, 0x000412,
0x000480, 0x000482, 0x000490, 0x000492, 0x002000, 0x002002, 0x002010, 0x002012, 0x002080, 0x002082, 0x002090, 0x002092,
0x002400, 0x002402, 0x002410, 0x002412, 0x002480, 0x002482, 0x002490, 0x002492, 0x010000, 0x010002, 0x010010, 0x010012,
0x010080, 0x010082, 0x010090, 0x010092, 0x010400, 0x010402, 0x010410, 0x010412, 0x010480, 0x010482, 0x010490, 0x010492,
0x012000, 0x012002, 0x012010, 0x012012, 0x012080, 0x012082, 0x012090, 0x012092, 0x012400, 0x012402, 0x012410, 0x012412,
0x012480, 0x012482, 0x012490, 0x012492, 0x080000, 0x080002, 0x080010, 0x080012, 0x080080, 0x080082, 0x080090, 0x080092,
0x080400, 0x080402, 0x080410, 0x080412, 0x080480, 0x080482, 0x080490, 0x080492, 0x082000, 0x082002, 0x082010, 0x082012,
0x082080, 0x082082, 0x082090, 0x082092, 0x082400, 0x082402, 0x082410, 0x082412, 0x082480, 0x082482, 0x082490, 0x082492,
0x090000, 0x090002, 0x090010, 0x090012, 0x090080, 0x090082, 0x090090, 0x090092, 0x090400, 0x090402, 0x090410, 0x090412,
0x090480, 0x090482, 0x090490, 0x090492, 0x092000, 0x092002, 0x092010, 0x092012, 0x092080, 0x092082, 0x092090, 0x092092,
0x092400, 0x092402, 0x092410, 0x092412, 0x092480, 0x092482, 0x092490, 0x092492, 0x400000, 0x400002, 0x400010, 0x400012,
0x400080, 0x400082, 0x400090, 0x400092, 0x400400, 0x400402, 0x400410, 0x400412, 0x400480, 0x400482, 0x400490, 0x400492,
0x402000, 0x402002, 0x402010, 0x402012, 0x402080, 0x402082, 0x402090, 0x402092, 0x402400, 0x402402, 0x402410, 0x402412,
0x402480, 0x402482, 0x402490, 0x402492, 0x410000, 0x410002, 0x410010, 0x410012, 0x410080, 0x410082, 0x410090, 0x410092,
0x410400, 0x410402, 0x410410, 0x410412, 0x410480, 0x410482, 0x410490, 0x410492, 0x412000, 0x412002, 0x412010, 0x412012,
0x412080, 0x412082, 0x412090, 0x412092, 0x412400, 0x412402, 0x412410, 0x412412, 0x412480, 0x412482, 0x412490, 0x412492,
0x480000, 0x480002, 0x480010, 0x480012, 0x480080, 0x480082, 0x480090, 0x480092, 0x480400, 0x480402, 0x480410, 0x480412,
0x480480, 0x480482, 0x480490, 0x480492, 0x482000, 0x482002, 0x482010, 0x482012, 0x482080, 0x482082, 0x482090, 0x482092,
0x482400, 0x482402, 0x482410, 0x482412, 0x482480, 0x482482, 0x482490, 0x482492, 0x490000, 0x490002, 0x490010, 0x490012,
0x490080, 0x490082, 0x490090, 0x490092, 0x490400, 0x490402, 0x490410, 0x490412, 0x490480, 0x490482, 0x490490, 0x490492,
0x492000, 0x492002, 0x492010, 0x492012, 0x492080, 0x492082, 0x492090, 0x492092, 0x492400, 0x492402, 0x492410, 0x492412,
0x492480, 0x492482, 0x492490, 0x492492};
const uint mortonZ[256] = {
0x000000, 0x000004, 0x000020, 0x000024, 0x000100, 0x000104, 0x000120, 0x000124, 0x000800, 0x000804, 0x000820, 0x000824,
0x000900, 0x000904, 0x000920, 0x000924, 0x004000, 0x004004, 0x004020, 0x004024, 0x004100, 0x004104, 0x004120, 0x004124,
0x004800, 0x004804, 0x004820, 0x004824, 0x004900, 0x004904, 0x004920, 0x004924, 0x020000, 0x020004, 0x020020, 0x020024,
0x020100, 0x020104, 0x020120, 0x020124, 0x020800, 0x020804, 0x020820, 0x020824, 0x020900, 0x020904, 0x020920, 0x020924,
0x024000, 0x024004, 0x024020, 0x024024, 0x024100, 0x024104, 0x024120, 0x024124, 0x024800, 0x024804, 0x024820, 0x024824,
0x024900, 0x024904, 0x024920, 0x024924, 0x100000, 0x100004, 0x100020, 0x100024, 0x100100, 0x100104, 0x100120, 0x100124,
0x100800, 0x100804, 0x100820, 0x100824, 0x100900, 0x100904, 0x100920, 0x100924, 0x104000, 0x104004, 0x104020, 0x104024,
0x104100, 0x104104, 0x104120, 0x104124, 0x104800, 0x104804, 0x104820, 0x104824, 0x104900, 0x104904, 0x104920, 0x104924,
0x120000, 0x120004, 0x120020, 0x120024, 0x120100, 0x120104, 0x120120, 0x120124, 0x120800, 0x120804, 0x120820, 0x120824,
0x120900, 0x120904, 0x120920, 0x120924, 0x124000, 0x124004, 0x124020, 0x124024, 0x124100, 0x124104, 0x124120, 0x124124,
0x124800, 0x124804, 0x124820, 0x124824, 0x124900, 0x124904, 0x124920, 0x124924, 0x800000, 0x800004, 0x800020, 0x800024,
0x800100, 0x800104, 0x800120, 0x800124, 0x800800, 0x800804, 0x800820, 0x800824, 0x800900, 0x800904, 0x800920, 0x800924,
0x804000, 0x804004, 0x804020, 0x804024, 0x804100, 0x804104, 0x804120, 0x804124, 0x804800, 0x804804, 0x804820, 0x804824,
0x804900, 0x804904, 0x804920, 0x804924, 0x820000, 0x820004, 0x820020, 0x820024, 0x820100, 0x820104, 0x820120, 0x820124,
0x820800, 0x820804, 0x820820, 0x820824, 0x820900, 0x820904, 0x820920, 0x820924, 0x824000, 0x824004, 0x824020, 0x824024,
0x824100, 0x824104, 0x824120, 0x824124, 0x824800, 0x824804, 0x824820, 0x824824, 0x824900, 0x824904, 0x824920, 0x824924,
0x900000, 0x900004, 0x900020, 0x900024, 0x900100, 0x900104, 0x900120, 0x900124, 0x900800, 0x900804, 0x900820, 0x900824,
0x900900, 0x900904, 0x900920, 0x900924, 0x904000, 0x904004, 0x904020, 0x904024, 0x904100, 0x904104, 0x904120, 0x904124,
0x904800, 0x904804, 0x904820, 0x904824, 0x904900, 0x904904, 0x904920, 0x904924, 0x920000, 0x920004, 0x920020, 0x920024,
0x920100, 0x920104, 0x920120, 0x920124, 0x920800, 0x920804, 0x920820, 0x920824, 0x920900, 0x920904, 0x920920, 0x920924,
0x924000, 0x924004, 0x924020, 0x924024, 0x924100, 0x924104, 0x924120, 0x924124, 0x924800, 0x924804, 0x924820, 0x924824,
0x924900, 0x924904, 0x924920, 0x924924};

#define startdetail 10
#define maxdetail 15
#define mindetail 1
#define steps 1000
#define emptycells 0.5
#define subdivisions 0.90
#define counter 0
#define lastmouse 1
#define clickray 4
#define detaildata 5
#define datapixels 6
#define cube_nothing 0
#define cube_octree 1
#define cube_empty 2
#define cube_brick 3
#define epsilon max(0.001*exp(-float(detail)), 0.0000001)


layout (std140, binding = 0) uniform shader_data {
	vec4 buf[4096]; // pack any data into vec4 to make full use of the aligned storage
};
layout (location = 0) in vec2 resIn;
layout (location = 1) in float timeIn;
layout (location = 2) in float dtIn;
layout (location = 3) in vec4 camPosIn;
layout (location = 4) in vec4 camRotIn;
layout (location = 5) in vec4 controlsIn;
layout (location = 0) out vec4 fsOut;
layout (binding = 0) uniform sampler2D dataTex;


void morton32(out uint morton, uint x, uint y, uint z){
	morton = 0;
	morton =  mortonZ[(z >> 16) & 0xFFu] | mortonY[(y >> 16) & 0xFFu] | mortonX[(x >> 16) & 0xFFu];
	morton = morton << 48 | mortonZ[(z >> 8) & 0xFFu] | mortonY[(y >> 8) & 0xFFu] | mortonX[(x >> 8) & 0xFFu];
	morton = morton << 24 | mortonZ[z & 0xFFu] | mortonY[y & 0xFFu] | mortonX[x & 0xFFu];
}
void morton8(out uint morton, uint x, uint y, uint z) { morton = mortonZ[z] | mortonY[y] | mortonX[x]; }
void mortonVoxelDword(out uint mortonVoxel, uint uDwordIdx) {
	mortonVoxel = floatBitsToUint(buf[uDwordIdx / 4][uDwordIdx % 4]);
}

void voxelDwordGet(out uint voxel, uint x, uint y, uint z) {
	uint mortonIdx;
	morton32(mortonIdx, x, y, z);
	mortonVoxelDword(voxel, mortonIdx);
}
bool vdGetIsFilled(uint voxel) { return bool(voxel & 0x100u); }
float vdGetRed(uint voxel) { return float((voxel & 0x1C00u) >> 10u) / 7.0; }
float vdGetGreen(uint voxel) { return float((voxel & 0xE000u) >> 13u) / 7.0; }
float vdGetBlue(uint voxel) { return float((voxel & 0x70000u) >> 16u) / 7.0; }


float rnd(vec4 v) { return fract(4e4*sin(dot(v, vec4(13.46, 41.74, -73.36, 14.24))+17.34)); }
int getvoxel(vec3 p, float size) { // 0 empty, 1 subdivide, 2 full
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

uint fetchVoxel(vec3 p, float size) {
	uvec3 pos = uvec3(p);
	uint voxel;
	voxelDwordGet(voxel, pos.x, pos.y, pos.z);
	bool isFilled = vdGetIsFilled(voxel);
	if (isFilled) { return 2; }
	return 0;
}

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
//			voxelstate1 = int(fetchVoxel(fro, size));
		}
		
		bool isnothing = recursions0 < recursions || voxelstate == cube_nothing;
		
		if (isnothing) {
			voxelstate = voxelstate1;
		}
		
		if (recursions == maxdetail-1 && voxelstate == cube_octree) {
			voxelstate = cube_empty;
		}
		// up level
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
		// subdivide
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
			
			// find which of the 8 voxels i will enter
			vec3 mask2 = step(vec3(size), lro);
			fro += mask2*size;
			lro -= mask2*size;
			hit = voxel(lro, rd, size);
		}
		// move forward
		else if (voxelstate == cube_nothing || voxelstate == cube_empty)
		{
			// raycast and find distance to nearest voxel surface in ray direction
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
	vec2 ndcPointAt = vec2(0);
	
	int detail = int(getdata(detaildata).x);
	vec4 cray = getdata(clickray);
	vec4 lastMouse = getdata(lastmouse);
	vec4 counterdata = getdata(counter);
	
//	bool click = lastMouse.z > 0.0 && camRotIn.z < 0.0 && length(lastMouse.xy-lastMouse.zw)<10.0;
	bool click = camRotIn.z < 0.0;
	bool erase = camRotIn.w < 0.0;
	vec3 mpos = cray.xyz;
	if (any(greaterThan(abs(mpos-0.5), vec3(0.5)))) click = false;
	
	bool refresh = (resIn.xy!=counterdata.zw);
	
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
	} else if (index == lastmouse) {
		return camRotIn;
	} else if (index == clickray) {
		
		if (refresh) {
			return vec4(0);
		}
//		if (camRotIn.z > 0.0 && lastMouse.z < 0.0) {
		if (camRotIn.z > 0.0) {
			
			vec3 ro = camPosIn.xyz;
			vec3 rd = normalize(vec3(ndcPointAt, 1));
			rd.zy *= mat2(cos(camRotIn.y), -sin(camRotIn.y), sin(camRotIn.y), cos(camRotIn.y));
			rd.zx *= mat2(cos(camRotIn.x), -sin(camRotIn.x), sin(camRotIn.x), cos(camRotIn.x));
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
		
		float keys = float(controlsIn.y < 0.0) - float(controlsIn.x < 0.0);
		data.x = clamp(data.x + keys, float(mindetail), float(maxdetail));
		
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