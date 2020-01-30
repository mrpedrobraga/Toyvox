#version 420 core

layout (location = 0) in vec2 resIn;
layout (location = 1) in float timeIn;
layout (location = 2) in float dtIn;
layout (location = 0) out vec4 fsOut;

layout (std140, binding = 0) uniform shader_data {
	vec4 buf[4096]; // pack any data into vec4 to make full use of the aligned storage
};

float posX = 0.5, posY = 0.5;  //x and y start position
float dirX = -1.0, dirY = 0.0; //initial direction vector
float planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane

float w = 1.0;
float h = 1.0;

int getWallMap(int x, int y){
	int w = 0;
	if(x == 0 && y == 0){w = 0;}if(x == 0 && y == 1){w = 0;}if(x == 0 && y == 2){w = 0;}if(x == 0 && y == 3){w = 0;}if(x == 0 && y == 4){w = 0;}
	if(x == 1 && y == 0){w = 0;}if(x == 1 && y == 1){w = 0;}if(x == 1 && y == 2){w = 3;}if(x == 1 && y == 3){w = 1;}if(x == 1 && y == 4){w = 0;}
	if(x == 2 && y == 0){w = 0;}if(x == 2 && y == 1){w = 2;}if(x == 2 && y == 2){w = 0;}if(x == 2 && y == 3){w = 4;}if(x == 2 && y == 4){w = 0;}
	if(x == 3 && y == 0){w = 0;}if(x == 3 && y == 1){w = 1;}if(x == 3 && y == 2){w = 1;}if(x == 3 && y == 3){w = 1;}if(x == 3 && y == 4){w = 0;}
	if(x == 4 && y == 0){w = 0;}if(x == 4 && y == 1){w = 0;}if(x == 4 && y == 2){w = 0;}if(x == 4 && y == 3){w = 0;}if(x == 4 && y == 4){w = 0;}
	return w;
}

void main() {
	vec2 p = ( gl_FragCoord.xy / resIn.xy );
	p.y = 1.0-p.y;
	
	vec3 col = vec3( 0.0 );
	
	float time = timeIn;
	float oldDirX = dirX;
	dirX = dirX * cos(-time) - dirY * sin(-time);
	dirY = oldDirX * sin(-time) + dirY * cos(-time);
	float oldPlaneX = planeX;
	planeX = planeX * cos(-time) - planeY * sin(-time);
	planeY = oldPlaneX * sin(-time) + planeY * cos(-time);
	
	//calculate ray position and direction
	float cameraX = 2.0 * p.x / w - 1.0; //x-coordinate in camera space
	float rayPosX = posX;
	float rayPosY = posY;
	float rayDirX = dirX + planeX * cameraX;
	float rayDirY = dirY + planeY * cameraX;
	//which box of the map we're in
	int mapX = int(rayPosX);
	int mapY = int(rayPosY);
	
	//length of ray from current position to next x or y-side
	float sideDistX;
	float sideDistY;
	
	//length of ray from one x or y-side to next x or y-side
	float deltaDistX = sqrt(1.0 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
	float deltaDistY = sqrt(1.0 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
	float perpWallDist;
	
	//what direction to step in x or y-direction (either +1 or -1)
	int stepX;
	int stepY;
	
	int hit = 0; //was there a wall hit?
	int side = 0; //was a NS or a EW wall hit?
	//calculate step and initial sideDist
	if (rayDirX < 0.0){
		stepX = -1;
		sideDistX = (rayPosX - float(mapX)) * deltaDistX;
	}else{
		stepX = 1;
		sideDistX = (float(mapX) + 1.0 - rayPosX) * deltaDistX;
	}
	
	if (rayDirY < 0.0){
		stepY = -1;
		sideDistY = (rayPosY - float(mapY)) * deltaDistY;
	}else{
		stepY = 1;
		sideDistY = (float(mapY) + 1.0 - rayPosY) * deltaDistY;
	}
	
	//perform DDA
	for(int i = 0; i<10; i++){
		if (hit == 0){
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY){
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}else{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if (getWallMap(mapX,mapY) >= 1) hit = 1;
		}
	}
	
	//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
	if (side == 0) perpWallDist = (float(mapX) - rayPosX + (1.0 - float(stepX)) / 2.0) / rayDirX;
	else           perpWallDist = (float(mapY) - rayPosY + (1.0 - float(stepY)) / 2.0) / rayDirY;
	
	//Calculate height of line to draw on screen
	float lineHeight = h / perpWallDist;
	
	//calculate lowest and highest pixel to fill in current stripe
	float drawStart = -lineHeight / 2.0 + h / 2.0;
	if(drawStart < 0.0)drawStart = 0.0;
	float drawEnd = lineHeight / 2.0 + h / 2.0;
	if(drawEnd >= h)drawEnd = h - 1.0;
	
	//choose wall color
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;
	
	int wall = getWallMap(mapX,mapY);
	if(wall == 1){
		r = 1.0;
	} else if(wall == 2) {
		g = 1.0;
	} else if(wall == 3) {
		b = 1.0;
	} else if(wall == 4) {
		r = 1.0;
		g = 1.0;
		b = 1.0;
	} else {
		r = 1.0;
		g = 1.0;
	}
	
	//give x and y sides different brightness
	if (side == 1) {
		r = r/2.0;
		g = g/2.0;
		b = b/2.0;
	}
	
	// drawing line
	if((p.x <= p.x+0.01) && (p.y >= drawStart && p.y <= drawEnd)){
		col = vec3( r,g,b );
	}
	
	fsOut = vec4( col, 1.0);
}