#include "basics.h"
#include "primitives.h"
#include <float.h>


SDL_Color SDL_GetRender_SDL_Color( SDL_Renderer *R ){
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor( R, &r, &g, &b, &a );
	return(SDL_Color){ r, g, b, a };
}


void gp_draw_thickLine( SDL_Renderer *R, int ax, int ay, int bx, int by, float radius ){

	int subx = bx - ax;
	int suby = by - ay;
	double len = sqrt( (subx * subx) + (suby * suby) ) + FLT_MIN;
	int dx = lrint(radius * subx / len);
	int dy = lrint(radius * suby / len);

	SDL_Color C = SDL_GetRender_SDL_Color( R );

	SDL_Vertex verts[4];
	verts[0] = (SDL_Vertex){ { ax + dy, ay - dx }, C, {0,0} };
	verts[1] = (SDL_Vertex){ { ax - dy, ay + dx }, C, {0,0} };
	verts[2] = (SDL_Vertex){ { bx - dy, by + dx }, C, {0,0} };
	verts[3] = (SDL_Vertex){ { bx + dy, by - dx }, C, {0,0} };

	int indices[6] = { 0, 1, 3, 1, 2, 3 };

	SDL_RenderGeometry( R, NULL, verts, 4, indices, 6 );
}

void gp_draw_roundedThickLine( SDL_Renderer *R, int ax, int ay, int bx, int by, float radius ){
	gp_draw_thickLine( R, ax, ay, bx, by, radius );
	gp_fill_fastcircle( R, ax, ay, radius );
	gp_fill_fastcircle( R, bx, by, radius );
}


void gp_draw_arrow( SDL_Renderer *R, int ax, int ay, int bx, int by, float LT, float HB, float HH ){

	int subx = bx - ax;
	int suby = by - ay;
	double len = sqrt( (subx * subx) + (suby * suby) ) + FLT_MIN;
	double nx = subx / len;
	double ny = suby / len;
	float ht = LT * 0.5;
	int ldx = lrint(ht * nx);
	int ldy = lrint(ht * ny);
	float hHB = HB * 0.5;
	int hdx = lrint(hHB * nx);
	int hdy = lrint(hHB * ny);
	int cx = bx - lrint(HH * nx);
	int cy = by - lrint(HH * ny);

	SDL_Color C = SDL_GetRender_SDL_Color( R );

	SDL_Vertex verts[7];
	verts[0] = (SDL_Vertex){ { ax + ldy, ay - ldx }, C, {0,0} };
	verts[1] = (SDL_Vertex){ { ax - ldy, ay + ldx }, C, {0,0} };
	verts[2] = (SDL_Vertex){ { cx - ldy, cy + ldx }, C, {0,0} };
	verts[3] = (SDL_Vertex){ { cx + ldy, cy - ldx }, C, {0,0} };

	verts[4] = (SDL_Vertex){ { cx + hdy, cy - hdx }, C, {0,0} };
	verts[5] = (SDL_Vertex){ { cx - hdy, cy + hdx }, C, {0,0} };
	verts[6] = (SDL_Vertex){ { bx,       by       }, C, {0,0} };

	int indices[9] = { 0, 1, 3, 1, 2, 3, 4, 5, 6 };

	SDL_RenderGeometry( R, NULL, verts, 7, indices, 9 );
}


void gp_draw_bezier1( SDL_Renderer *R, vec2d *a1, vec2d *a2, vec2d *c, int res ){
	vec2d prev = *a1;
	float t = 1.0 / res;
	for (int i = 1; i <= res; ++i){
		float amt = i * t;
		vec2d inter1 = lerp_v2d( *a1, *c, amt );
		vec2d inter2 = lerp_v2d( *c, *a2, amt );
		vec2d point  = lerp_v2d( inter1, inter2, amt );
		SDL_RenderDrawLineF(R, prev.x, prev.y, point.x, point.y );
		prev = point;
	}
}

void gp_draw_bezier2( SDL_Renderer *R, vec2d *a1, vec2d *a2, vec2d *c1, vec2d *c2, int res ){
	vec2d prev = *a1;
	float t = 1.0 / res;
	for (int i = 1; i <= res; ++i){
		float amt = i * t;
		vec2d inter01 = lerp_v2d( *a1, *c1, amt );
		vec2d inter02 = lerp_v2d( *c1, *c2, amt );
		vec2d inter03 = lerp_v2d( *c2, *a2, amt );
		vec2d inter11 = lerp_v2d( inter01, inter02, amt );
		vec2d inter12 = lerp_v2d( inter02, inter03, amt );
		vec2d point   = lerp_v2d( inter11, inter12, amt );
		SDL_RenderDrawLineF( R, prev.x, prev.y, point.x, point.y );
		prev = point;
	}
}





//https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
void gp_draw_circle(SDL_Renderer *R, int x, int y, int radius){

	int offsetx, offsety, d;
	int status;

	//CHECK_RENDERER_MAGIC(R, -1);

	offsetx = 0;
	offsety = radius;
	d = radius -1;
	//status = 0;

	while (offsety >= offsetx) {
		SDL_RenderDrawPoint(R, x + offsetx, y + offsety); //status |= 
		SDL_RenderDrawPoint(R, x + offsety, y + offsetx); //status |= 
		SDL_RenderDrawPoint(R, x - offsetx, y + offsety); //status |= 
		SDL_RenderDrawPoint(R, x - offsety, y + offsetx); //status |= 
		SDL_RenderDrawPoint(R, x + offsetx, y - offsety); //status |= 
		SDL_RenderDrawPoint(R, x + offsety, y - offsetx); //status |= 
		SDL_RenderDrawPoint(R, x - offsetx, y - offsety); //status |= 
		SDL_RenderDrawPoint(R, x - offsety, y - offsetx); //status |= 

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2*offsetx) {
			d -= 2*offsetx + 1;
			offsetx +=1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}

	//return status;
}
//https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
void gp_fill_circle(SDL_Renderer *R, int x, int y, int radius){
	int offsetx, offsety, d;
	int status;

	//CHECK_RENDERER_MAGIC(R, -1);

	offsetx = 0;
	offsety = radius;
	d = radius -1;
	//status = 0;

	while (offsety >= offsetx) {

		SDL_RenderDrawLine(R, x - offsety, y + offsetx,     //status |= 
						   x + offsety, y + offsetx);          
		SDL_RenderDrawLine(R, x - offsetx, y + offsety,     //status |= 
						   x + offsetx, y + offsety);          
		SDL_RenderDrawLine(R, x - offsetx, y - offsety,     //status |= 
						   x + offsetx, y - offsety);          
		SDL_RenderDrawLine(R, x - offsety, y - offsetx,     //status |= 
						   x + offsety, y - offsetx);          

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2*offsetx) {
			d -= 2*offsetx + 1;
			offsetx +=1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}

	//return status;
}

void gp_draw_8circle(SDL_Renderer *R, int x, int y, int radius){
	const float trig = 0.707107*radius;
	SDL_RenderDrawLineF( R, x + radius, y         , x + trig  , y + trig   );
	SDL_RenderDrawLineF( R, x + trig  , y + trig  , x         , y + radius );
	SDL_RenderDrawLineF( R, x         , y + radius, x - trig  , y + trig   );
	SDL_RenderDrawLineF( R, x - trig  , y + trig  , x - radius, y          );
	SDL_RenderDrawLineF( R, x - radius, y         , x - trig  , y - trig   );
	SDL_RenderDrawLineF( R, x - trig  , y - trig  , x         , y - radius );
	SDL_RenderDrawLineF( R, x         , y - radius, x + trig  , y - trig   );
	SDL_RenderDrawLineF( R, x + trig  , y - trig  , x + radius, y          );
}
void gp_drawthick_8circle(SDL_Renderer *R, int x, int y, int C_radius, float L_radius ){
	SDL_Color C = SDL_GetRender_SDL_Color( R );
	float rad = C_radius + L_radius;
	float trig = 0.707107*rad;
	SDL_Vertex verts[16];
	verts[ 0] = (SDL_Vertex){ { x + rad , y        }, C, {0,0} };
	verts[ 1] = (SDL_Vertex){ { x + trig, y + trig }, C, {0,0} };
	verts[ 2] = (SDL_Vertex){ { x       , y + rad  }, C, {0,0} };
	verts[ 3] = (SDL_Vertex){ { x - trig, y + trig }, C, {0,0} };
	verts[ 4] = (SDL_Vertex){ { x - rad , y        }, C, {0,0} };
	verts[ 5] = (SDL_Vertex){ { x - trig, y - trig }, C, {0,0} };
	verts[ 6] = (SDL_Vertex){ { x       , y - rad  }, C, {0,0} };
	verts[ 7] = (SDL_Vertex){ { x + trig, y - trig }, C, {0,0} };
	rad = C_radius - L_radius;
	trig = 0.707107*rad;
	verts[ 8] = (SDL_Vertex){ { x + rad , y        }, C, {0,0} };
	verts[ 9] = (SDL_Vertex){ { x + trig, y + trig }, C, {0,0} };
	verts[10] = (SDL_Vertex){ { x       , y + rad  }, C, {0,0} };
	verts[11] = (SDL_Vertex){ { x - trig, y + trig }, C, {0,0} };
	verts[12] = (SDL_Vertex){ { x - rad , y        }, C, {0,0} };
	verts[13] = (SDL_Vertex){ { x - trig, y - trig }, C, {0,0} };
	verts[14] = (SDL_Vertex){ { x       , y - rad  }, C, {0,0} };
	verts[15] = (SDL_Vertex){ { x + trig, y - trig }, C, {0,0} };
	int indices[48] = { 0, 1, 8, 1, 9, 8, 1, 2, 9, 2, 10, 9, 2, 3, 
		10, 3, 11, 10, 3, 4, 11, 4, 12, 11, 4, 5, 12, 5, 13, 12, 5, 
		6, 13, 6, 14, 13, 6, 7, 14, 7, 15, 14, 7, 0, 15, 0, 8, 15 };
	SDL_RenderGeometry( R, NULL, verts, 16, indices, 48 );
}
void gp_fill_8circle(SDL_Renderer *R, int x, int y, int radius){
		SDL_Color C = SDL_GetRender_SDL_Color( R );
		const float trig = { 0.707107*radius};
		SDL_Vertex verts[8];
		verts[0] = (SDL_Vertex){ { x + radius, y          }, C, {0,0} };
		verts[1] = (SDL_Vertex){ { x + trig  , y + trig   }, C, {0,0} };
		verts[2] = (SDL_Vertex){ { x         , y + radius }, C, {0,0} };
		verts[3] = (SDL_Vertex){ { x - trig  , y + trig   }, C, {0,0} };
		verts[4] = (SDL_Vertex){ { x - radius, y          }, C, {0,0} };
		verts[5] = (SDL_Vertex){ { x - trig  , y - trig   }, C, {0,0} };
		verts[6] = (SDL_Vertex){ { x         , y - radius }, C, {0,0} };
		verts[7] = (SDL_Vertex){ { x + trig  , y - trig   }, C, {0,0} };
		int indices[18] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7 };
		SDL_RenderGeometry( R, NULL, verts, 8, indices, 18 );
}

void gp_draw_16circle(SDL_Renderer *R, int x, int y, int radius){
		const  float trig [3] = {  0.3826834324*radius, 0.7071067812*radius, 0.9238795325*radius };
		SDL_RenderDrawLineF( R, x + radius , y          , x + trig[2], y + trig[0] );
		SDL_RenderDrawLineF( R, x + trig[2], y + trig[0], x + trig[1], y + trig[1] );
		SDL_RenderDrawLineF( R, x + trig[1], y + trig[1], x + trig[0], y + trig[2] );
		SDL_RenderDrawLineF( R, x + trig[0], y + trig[2], x          , y + radius  );
		SDL_RenderDrawLineF( R, x          , y + radius , x - trig[0], y + trig[2] );
		SDL_RenderDrawLineF( R, x - trig[0], y + trig[2], x - trig[1], y + trig[1] );
		SDL_RenderDrawLineF( R, x - trig[1], y + trig[1], x - trig[2], y + trig[0] );
		SDL_RenderDrawLineF( R, x - trig[2], y + trig[0], x - radius , y           );
		SDL_RenderDrawLineF( R, x - radius , y          , x - trig[2], y - trig[0] );
		SDL_RenderDrawLineF( R, x - trig[2], y - trig[0], x - trig[1], y - trig[1] );
		SDL_RenderDrawLineF( R, x - trig[1], y - trig[1], x - trig[0], y - trig[2] );
		SDL_RenderDrawLineF( R, x - trig[0], y - trig[2], x          , y - radius  );
		SDL_RenderDrawLineF( R, x          , y - radius , x + trig[0], y - trig[2] );
		SDL_RenderDrawLineF( R, x + trig[0], y - trig[2], x + trig[1], y - trig[1] );
		SDL_RenderDrawLineF( R, x + trig[1], y - trig[1], x + trig[2], y - trig[0] );
		SDL_RenderDrawLineF( R, x + trig[2], y - trig[0], x + radius , y           );
}
void gp_drawthick_16circle(SDL_Renderer *R, int x, int y, int C_radius, float L_radius ){
	SDL_Color C = SDL_GetRender_SDL_Color( R );
	float rad = C_radius + L_radius;
	float trig [3] = { 0.382683*rad, 0.707107*rad, 0.923880*rad };
	SDL_Vertex verts[32];
	verts[ 0] = (SDL_Vertex){ { x + rad    , y           }, C, {0,0} };
	verts[ 1] = (SDL_Vertex){ { x + trig[2], y + trig[0] }, C, {0,0} };
	verts[ 2] = (SDL_Vertex){ { x + trig[1], y + trig[1] }, C, {0,0} };
	verts[ 3] = (SDL_Vertex){ { x + trig[0], y + trig[2] }, C, {0,0} };
	verts[ 4] = (SDL_Vertex){ { x          , y + rad     }, C, {0,0} };
	verts[ 5] = (SDL_Vertex){ { x - trig[0], y + trig[2] }, C, {0,0} };
	verts[ 6] = (SDL_Vertex){ { x - trig[1], y + trig[1] }, C, {0,0} };
	verts[ 7] = (SDL_Vertex){ { x - trig[2], y + trig[0] }, C, {0,0} };
	verts[ 8] = (SDL_Vertex){ { x - rad    , y           }, C, {0,0} };
	verts[ 9] = (SDL_Vertex){ { x - trig[2], y - trig[0] }, C, {0,0} };
	verts[10] = (SDL_Vertex){ { x - trig[1], y - trig[1] }, C, {0,0} };
	verts[11] = (SDL_Vertex){ { x - trig[0], y - trig[2] }, C, {0,0} };
	verts[12] = (SDL_Vertex){ { x          , y - rad     }, C, {0,0} };
	verts[13] = (SDL_Vertex){ { x + trig[0], y - trig[2] }, C, {0,0} };
	verts[14] = (SDL_Vertex){ { x + trig[1], y - trig[1] }, C, {0,0} };
	verts[15] = (SDL_Vertex){ { x + trig[2], y - trig[0] }, C, {0,0} };
	rad = C_radius - L_radius;
	trig[0] = 0.382683*rad; trig[1] = 0.707107*rad; trig[2] = 0.923880*rad;
	verts[16] = (SDL_Vertex){ { x + rad    , y           }, C, {0,0} };
	verts[17] = (SDL_Vertex){ { x + trig[2], y + trig[0] }, C, {0,0} };
	verts[18] = (SDL_Vertex){ { x + trig[1], y + trig[1] }, C, {0,0} };
	verts[19] = (SDL_Vertex){ { x + trig[0], y + trig[2] }, C, {0,0} };
	verts[20] = (SDL_Vertex){ { x          , y + rad     }, C, {0,0} };
	verts[21] = (SDL_Vertex){ { x - trig[0], y + trig[2] }, C, {0,0} };
	verts[22] = (SDL_Vertex){ { x - trig[1], y + trig[1] }, C, {0,0} };
	verts[23] = (SDL_Vertex){ { x - trig[2], y + trig[0] }, C, {0,0} };
	verts[24] = (SDL_Vertex){ { x - rad    , y           }, C, {0,0} };
	verts[25] = (SDL_Vertex){ { x - trig[2], y - trig[0] }, C, {0,0} };
	verts[26] = (SDL_Vertex){ { x - trig[1], y - trig[1] }, C, {0,0} };
	verts[27] = (SDL_Vertex){ { x - trig[0], y - trig[2] }, C, {0,0} };
	verts[28] = (SDL_Vertex){ { x          , y - rad     }, C, {0,0} };
	verts[29] = (SDL_Vertex){ { x + trig[0], y - trig[2] }, C, {0,0} };
	verts[30] = (SDL_Vertex){ { x + trig[1], y - trig[1] }, C, {0,0} };
	verts[31] = (SDL_Vertex){ { x + trig[2], y - trig[0] }, C, {0,0} };
	int indices[96] = { 0, 1, 16, 1, 17, 16, 1, 2, 17, 2, 18, 17, 2, 3, 
		18, 3, 19, 18, 3, 4, 19, 4, 20, 19, 4, 5, 20, 5, 21, 20, 5, 6, 
		21, 6, 22, 21, 6, 7, 22, 7, 23, 22, 7, 8, 23, 8, 24, 23, 8, 9, 
		24, 9, 25, 24, 9, 10, 25, 10, 26, 25, 10, 11, 26, 11, 27, 26, 
		11, 12, 27, 12, 28, 27, 12, 13, 28, 13, 29, 28, 13, 14, 29, 14, 
		30, 29, 14, 15, 30, 15, 31, 30, 15, 0, 31, 0, 16, 31 };
	SDL_RenderGeometry( R, NULL, verts, 32, indices, 96 );
}
void gp_fill_16circle(SDL_Renderer *R, int x, int y, int radius){
	SDL_Color C = SDL_GetRender_SDL_Color( R );
	const float trig [3] = {  0.3826834324*radius, 0.7071067812*radius, 0.9238795325*radius };
	SDL_Vertex verts[16];
	verts[ 0] = (SDL_Vertex){ { x + radius , y           }, C, {0,0} };
	verts[ 1] = (SDL_Vertex){ { x + trig[2], y + trig[0] }, C, {0,0} };
	verts[ 2] = (SDL_Vertex){ { x + trig[1], y + trig[1] }, C, {0,0} };
	verts[ 3] = (SDL_Vertex){ { x + trig[0], y + trig[2] }, C, {0,0} };
	verts[ 4] = (SDL_Vertex){ { x          , y + radius  }, C, {0,0} };
	verts[ 5] = (SDL_Vertex){ { x - trig[0], y + trig[2] }, C, {0,0} };
	verts[ 6] = (SDL_Vertex){ { x - trig[1], y + trig[1] }, C, {0,0} };
	verts[ 7] = (SDL_Vertex){ { x - trig[2], y + trig[0] }, C, {0,0} };
	verts[ 8] = (SDL_Vertex){ { x - radius , y           }, C, {0,0} };
	verts[ 9] = (SDL_Vertex){ { x - trig[2], y - trig[0] }, C, {0,0} };
	verts[10] = (SDL_Vertex){ { x - trig[1], y - trig[1] }, C, {0,0} };
	verts[11] = (SDL_Vertex){ { x - trig[0], y - trig[2] }, C, {0,0} };
	verts[12] = (SDL_Vertex){ { x          , y - radius  }, C, {0,0} };
	verts[13] = (SDL_Vertex){ { x + trig[0], y - trig[2] }, C, {0,0} };
	verts[14] = (SDL_Vertex){ { x + trig[1], y - trig[1] }, C, {0,0} };
	verts[15] = (SDL_Vertex){ { x + trig[2], y - trig[0] }, C, {0,0} };
	int indices[42] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 
						9, 0, 9, 10, 0, 10, 11, 0, 11, 12, 0, 12, 13, 0, 13, 14, 0, 14, 15 };
	SDL_RenderGeometry( R, NULL, verts, 16, indices, 42 );
}

void gp_draw_24circle(SDL_Renderer *R, int x, int y, int radius){
		const float trig [5] = { 0.258819*radius, 0.500000*radius, 0.707107*radius, 
								 0.866025*radius, 0.965926*radius};
		SDL_RenderDrawLineF( R, x + radius , y          , x + trig[4], y + trig[0] );
		SDL_RenderDrawLineF( R, x + trig[4], y + trig[0], x + trig[3], y + trig[1] );
		SDL_RenderDrawLineF( R, x + trig[3], y + trig[1], x + trig[2], y + trig[2] );
		SDL_RenderDrawLineF( R, x + trig[2], y + trig[2], x + trig[1], y + trig[3] );
		SDL_RenderDrawLineF( R, x + trig[1], y + trig[3], x + trig[0], y + trig[4] );
		SDL_RenderDrawLineF( R, x + trig[0], y + trig[4], x          , y + radius  );
		SDL_RenderDrawLineF( R, x          , y + radius , x - trig[0], y + trig[4] );
		SDL_RenderDrawLineF( R, x - trig[0], y + trig[4], x - trig[1], y + trig[3] );
		SDL_RenderDrawLineF( R, x - trig[1], y + trig[3], x - trig[2], y + trig[2] );
		SDL_RenderDrawLineF( R, x - trig[2], y + trig[2], x - trig[3], y + trig[1] );
		SDL_RenderDrawLineF( R, x - trig[3], y + trig[1], x - trig[4], y + trig[0] );
		SDL_RenderDrawLineF( R, x - trig[4], y + trig[0], x - radius , y           );
		SDL_RenderDrawLineF( R, x - radius , y          , x - trig[4], y - trig[0] );
		SDL_RenderDrawLineF( R, x - trig[4], y - trig[0], x - trig[3], y - trig[1] );
		SDL_RenderDrawLineF( R, x - trig[3], y - trig[1], x - trig[2], y - trig[2] );
		SDL_RenderDrawLineF( R, x - trig[2], y - trig[2], x - trig[1], y - trig[3] );
		SDL_RenderDrawLineF( R, x - trig[1], y - trig[3], x - trig[0], y - trig[4] );
		SDL_RenderDrawLineF( R, x - trig[0], y - trig[4], x          , y - radius  );
		SDL_RenderDrawLineF( R, x          , y - radius , x + trig[0], y - trig[4] );
		SDL_RenderDrawLineF( R, x + trig[0], y - trig[4], x + trig[1], y - trig[3] );
		SDL_RenderDrawLineF( R, x + trig[1], y - trig[3], x + trig[2], y - trig[2] );
		SDL_RenderDrawLineF( R, x + trig[2], y - trig[2], x + trig[3], y - trig[1] );
		SDL_RenderDrawLineF( R, x + trig[3], y - trig[1], x + trig[4], y - trig[0] );
		SDL_RenderDrawLineF( R, x + trig[4], y - trig[0], x + radius , y           );
}
void gp_drawthick_24circle(SDL_Renderer *R, int x, int y, int C_radius, float L_radius ){
	SDL_Color C = SDL_GetRender_SDL_Color( R );
	float rad = C_radius + L_radius;
	float trig [5] = { 0.258819*rad, 0.500000*rad, 0.707107*rad, 0.866025*rad, 0.965926*rad};
	SDL_Vertex verts[48];
	verts[ 0] = (SDL_Vertex){ { x + rad    , y           }, C, {0,0} };
	verts[ 1] = (SDL_Vertex){ { x + trig[4], y + trig[0] }, C, {0,0} };
	verts[ 2] = (SDL_Vertex){ { x + trig[3], y + trig[1] }, C, {0,0} };
	verts[ 3] = (SDL_Vertex){ { x + trig[2], y + trig[2] }, C, {0,0} };
	verts[ 4] = (SDL_Vertex){ { x + trig[1], y + trig[3] }, C, {0,0} };
	verts[ 5] = (SDL_Vertex){ { x + trig[0], y + trig[4] }, C, {0,0} };
	verts[ 6] = (SDL_Vertex){ { x          , y + rad     }, C, {0,0} };
	verts[ 7] = (SDL_Vertex){ { x - trig[0], y + trig[4] }, C, {0,0} };
	verts[ 8] = (SDL_Vertex){ { x - trig[1], y + trig[3] }, C, {0,0} };
	verts[ 9] = (SDL_Vertex){ { x - trig[2], y + trig[2] }, C, {0,0} };
	verts[10] = (SDL_Vertex){ { x - trig[3], y + trig[1] }, C, {0,0} };
	verts[11] = (SDL_Vertex){ { x - trig[4], y + trig[0] }, C, {0,0} };
	verts[12] = (SDL_Vertex){ { x - rad    , y           }, C, {0,0} };
	verts[13] = (SDL_Vertex){ { x - trig[4], y - trig[0] }, C, {0,0} };
	verts[14] = (SDL_Vertex){ { x - trig[3], y - trig[1] }, C, {0,0} };
	verts[15] = (SDL_Vertex){ { x - trig[2], y - trig[2] }, C, {0,0} };
	verts[16] = (SDL_Vertex){ { x - trig[1], y - trig[3] }, C, {0,0} };
	verts[17] = (SDL_Vertex){ { x - trig[0], y - trig[4] }, C, {0,0} };
	verts[18] = (SDL_Vertex){ { x          , y - rad     }, C, {0,0} };
	verts[19] = (SDL_Vertex){ { x + trig[0], y - trig[4] }, C, {0,0} };
	verts[20] = (SDL_Vertex){ { x + trig[1], y - trig[3] }, C, {0,0} };
	verts[21] = (SDL_Vertex){ { x + trig[2], y - trig[2] }, C, {0,0} };
	verts[22] = (SDL_Vertex){ { x + trig[3], y - trig[1] }, C, {0,0} };
	verts[23] = (SDL_Vertex){ { x + trig[4], y - trig[0] }, C, {0,0} };
	rad = C_radius - L_radius;
	trig[0] = 0.258819*rad; trig[1] = 0.500000*rad; trig[2] = 0.707107*rad; 
	trig[3] = 0.866025*rad; trig[4] = 0.965926*rad;
	verts[24] = (SDL_Vertex){ { x + rad    , y           }, C, {0,0} };
	verts[25] = (SDL_Vertex){ { x + trig[4], y + trig[0] }, C, {0,0} };
	verts[26] = (SDL_Vertex){ { x + trig[3], y + trig[1] }, C, {0,0} };
	verts[27] = (SDL_Vertex){ { x + trig[2], y + trig[2] }, C, {0,0} };
	verts[28] = (SDL_Vertex){ { x + trig[1], y + trig[3] }, C, {0,0} };
	verts[29] = (SDL_Vertex){ { x + trig[0], y + trig[4] }, C, {0,0} };
	verts[30] = (SDL_Vertex){ { x          , y + rad     }, C, {0,0} };
	verts[31] = (SDL_Vertex){ { x - trig[0], y + trig[4] }, C, {0,0} };
	verts[32] = (SDL_Vertex){ { x - trig[1], y + trig[3] }, C, {0,0} };
	verts[33] = (SDL_Vertex){ { x - trig[2], y + trig[2] }, C, {0,0} };
	verts[34] = (SDL_Vertex){ { x - trig[3], y + trig[1] }, C, {0,0} };
	verts[35] = (SDL_Vertex){ { x - trig[4], y + trig[0] }, C, {0,0} };
	verts[36] = (SDL_Vertex){ { x - rad    , y           }, C, {0,0} };
	verts[37] = (SDL_Vertex){ { x - trig[4], y - trig[0] }, C, {0,0} };
	verts[38] = (SDL_Vertex){ { x - trig[3], y - trig[1] }, C, {0,0} };
	verts[39] = (SDL_Vertex){ { x - trig[2], y - trig[2] }, C, {0,0} };
	verts[40] = (SDL_Vertex){ { x - trig[1], y - trig[3] }, C, {0,0} };
	verts[41] = (SDL_Vertex){ { x - trig[0], y - trig[4] }, C, {0,0} };
	verts[42] = (SDL_Vertex){ { x          , y - rad     }, C, {0,0} };
	verts[43] = (SDL_Vertex){ { x + trig[0], y - trig[4] }, C, {0,0} };
	verts[44] = (SDL_Vertex){ { x + trig[1], y - trig[3] }, C, {0,0} };
	verts[45] = (SDL_Vertex){ { x + trig[2], y - trig[2] }, C, {0,0} };
	verts[46] = (SDL_Vertex){ { x + trig[3], y - trig[1] }, C, {0,0} };
	verts[47] = (SDL_Vertex){ { x + trig[4], y - trig[0] }, C, {0,0} };
	int indices[144] = { 0, 1, 24, 1, 25, 24, 1, 2, 25, 2, 26, 25, 2, 3, 
		26, 3, 27, 26, 3, 4, 27, 4, 28, 27, 4, 5, 28, 5, 29, 28, 5, 6, 29, 
		6, 30, 29, 6, 7, 30, 7, 31, 30, 7, 8, 31, 8, 32, 31, 8, 9, 32, 9, 
		33, 32, 9, 10, 33, 10, 34, 33, 10, 11, 34, 11, 35, 34, 11, 12, 35, 
		12, 36, 35, 12, 13, 36, 13, 37, 36, 13, 14, 37, 14, 38, 37, 14, 15, 
		38, 15, 39, 38, 15, 16, 39, 16, 40, 39, 16, 17, 40, 17, 41, 40, 17,
		 18, 41, 18, 42, 41, 18, 19, 42, 19, 43, 42, 19, 20, 43, 20, 44, 43, 
		 20, 21, 44, 21, 45, 44, 21, 22, 45, 22, 46, 45, 22, 23, 46, 23, 47, 
		 46, 23, 0, 47, 0, 24, 47 };
	SDL_RenderGeometry( R, NULL, verts, 48, indices, 144 );
}
void gp_fill_24circle(SDL_Renderer *R, int x, int y, int radius){
		SDL_Color C = SDL_GetRender_SDL_Color( R );
		const float trig [5] = { 0.258819*radius, 0.500000*radius, 0.707107*radius, 0.866025*radius, 0.965926*radius};
		SDL_Vertex verts[24];
		verts[ 0] = (SDL_Vertex){ { x + radius , y           }, C, {0,0} };
		verts[ 1] = (SDL_Vertex){ { x + trig[4], y + trig[0] }, C, {0,0} };
		verts[ 2] = (SDL_Vertex){ { x + trig[3], y + trig[1] }, C, {0,0} };
		verts[ 3] = (SDL_Vertex){ { x + trig[2], y + trig[2] }, C, {0,0} };
		verts[ 4] = (SDL_Vertex){ { x + trig[1], y + trig[3] }, C, {0,0} };
		verts[ 5] = (SDL_Vertex){ { x + trig[0], y + trig[4] }, C, {0,0} };
		verts[ 6] = (SDL_Vertex){ { x          , y + radius  }, C, {0,0} };
		verts[ 7] = (SDL_Vertex){ { x - trig[0], y + trig[4] }, C, {0,0} };
		verts[ 8] = (SDL_Vertex){ { x - trig[1], y + trig[3] }, C, {0,0} };
		verts[ 9] = (SDL_Vertex){ { x - trig[2], y + trig[2] }, C, {0,0} };
		verts[10] = (SDL_Vertex){ { x - trig[3], y + trig[1] }, C, {0,0} };
		verts[11] = (SDL_Vertex){ { x - trig[4], y + trig[0] }, C, {0,0} };
		verts[12] = (SDL_Vertex){ { x - radius , y           }, C, {0,0} };
		verts[13] = (SDL_Vertex){ { x - trig[4], y - trig[0] }, C, {0,0} };
		verts[14] = (SDL_Vertex){ { x - trig[3], y - trig[1] }, C, {0,0} };
		verts[15] = (SDL_Vertex){ { x - trig[2], y - trig[2] }, C, {0,0} };
		verts[16] = (SDL_Vertex){ { x - trig[1], y - trig[3] }, C, {0,0} };
		verts[17] = (SDL_Vertex){ { x - trig[0], y - trig[4] }, C, {0,0} };
		verts[18] = (SDL_Vertex){ { x          , y - radius  }, C, {0,0} };
		verts[19] = (SDL_Vertex){ { x + trig[0], y - trig[4] }, C, {0,0} };
		verts[20] = (SDL_Vertex){ { x + trig[1], y - trig[3] }, C, {0,0} };
		verts[21] = (SDL_Vertex){ { x + trig[2], y - trig[2] }, C, {0,0} };
		verts[22] = (SDL_Vertex){ { x + trig[3], y - trig[1] }, C, {0,0} };
		verts[23] = (SDL_Vertex){ { x + trig[4], y - trig[0] }, C, {0,0} };
		int indices[66] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 
			6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 11, 0, 11, 12, 0, 12, 
			13, 0, 13, 14, 0, 14, 15, 0, 15, 16, 0, 16, 17, 0, 17, 18, 0, 
			18, 19, 0, 19, 20, 0, 20, 21, 0, 21, 22, 0, 22, 23 };
		SDL_RenderGeometry( R, NULL, verts, 24, indices, 66 );
}

void gp_draw_36circle(SDL_Renderer *R, int x, int y, int radius){
		const float trig [8] = { 0.173648*radius, 0.342020*radius, 0.500000*radius, 0.642788*radius, 
								 0.766044*radius, 0.866025*radius, 0.939693*radius, 0.984808*radius};
		SDL_RenderDrawLineF( R, x + radius , y          , x + trig[7], y + trig[0] );
		SDL_RenderDrawLineF( R, x + trig[7], y + trig[0], x + trig[6], y + trig[1] );
		SDL_RenderDrawLineF( R, x + trig[6], y + trig[1], x + trig[5], y + trig[2] );
		SDL_RenderDrawLineF( R, x + trig[5], y + trig[2], x + trig[4], y + trig[3] );
		SDL_RenderDrawLineF( R, x + trig[4], y + trig[3], x + trig[3], y + trig[4] );
		SDL_RenderDrawLineF( R, x + trig[3], y + trig[4], x + trig[2], y + trig[5] );
		SDL_RenderDrawLineF( R, x + trig[2], y + trig[5], x + trig[1], y + trig[6] );
		SDL_RenderDrawLineF( R, x + trig[1], y + trig[6], x + trig[0], y + trig[7] );
		SDL_RenderDrawLineF( R, x + trig[0], y + trig[7], x          , y + radius  );
		SDL_RenderDrawLineF( R, x          , y + radius , x - trig[0], y + trig[7] );
		SDL_RenderDrawLineF( R, x - trig[0], y + trig[7], x - trig[1], y + trig[6] );
		SDL_RenderDrawLineF( R, x - trig[1], y + trig[6], x - trig[2], y + trig[5] );
		SDL_RenderDrawLineF( R, x - trig[2], y + trig[5], x - trig[3], y + trig[4] );
		SDL_RenderDrawLineF( R, x - trig[3], y + trig[4], x - trig[4], y + trig[3] );
		SDL_RenderDrawLineF( R, x - trig[4], y + trig[3], x - trig[5], y + trig[2] );
		SDL_RenderDrawLineF( R, x - trig[5], y + trig[2], x - trig[6], y + trig[1] );
		SDL_RenderDrawLineF( R, x - trig[6], y + trig[1], x - trig[7], y + trig[0] );
		SDL_RenderDrawLineF( R, x - trig[7], y + trig[0], x - radius , y           );
		SDL_RenderDrawLineF( R, x - radius , y          , x - trig[7], y - trig[0] );
		SDL_RenderDrawLineF( R, x - trig[7], y - trig[0], x - trig[6], y - trig[1] );
		SDL_RenderDrawLineF( R, x - trig[6], y - trig[1], x - trig[5], y - trig[2] );
		SDL_RenderDrawLineF( R, x - trig[5], y - trig[2], x - trig[4], y - trig[3] );
		SDL_RenderDrawLineF( R, x - trig[4], y - trig[3], x - trig[3], y - trig[4] );
		SDL_RenderDrawLineF( R, x - trig[3], y - trig[4], x - trig[2], y - trig[5] );
		SDL_RenderDrawLineF( R, x - trig[2], y - trig[5], x - trig[1], y - trig[6] );
		SDL_RenderDrawLineF( R, x - trig[1], y - trig[6], x - trig[0], y - trig[7] );
		SDL_RenderDrawLineF( R, x - trig[0], y - trig[7], x          , y - radius  );
		SDL_RenderDrawLineF( R, x          , y - radius , x + trig[0], y - trig[7] );
		SDL_RenderDrawLineF( R, x + trig[0], y - trig[7], x + trig[1], y - trig[6] );
		SDL_RenderDrawLineF( R, x + trig[1], y - trig[6], x + trig[2], y - trig[5] );
		SDL_RenderDrawLineF( R, x + trig[2], y - trig[5], x + trig[3], y - trig[4] );
		SDL_RenderDrawLineF( R, x + trig[3], y - trig[4], x + trig[4], y - trig[3] );
		SDL_RenderDrawLineF( R, x + trig[4], y - trig[3], x + trig[5], y - trig[2] );
		SDL_RenderDrawLineF( R, x + trig[5], y - trig[2], x + trig[6], y - trig[1] );
		SDL_RenderDrawLineF( R, x + trig[6], y - trig[1], x + trig[7], y - trig[0] );
		SDL_RenderDrawLineF( R, x + trig[7], y - trig[0], x + radius , y           );
}
void gp_drawthick_36circle(SDL_Renderer *R, int x, int y, int C_radius, float L_radius ){
		SDL_Color C = SDL_GetRender_SDL_Color( R );
		float rad = C_radius + L_radius;
		float trig [8] = { 0.173648*rad, 0.342020*rad, 0.500000*rad, 0.642788*rad, 
						   0.766044*rad, 0.866025*rad, 0.939693*rad, 0.984808*rad};
		SDL_Vertex verts[72];
		verts[ 0] = (SDL_Vertex){ { x + rad    , y           }, C, {0,0} };
		verts[ 1] = (SDL_Vertex){ { x + trig[7], y + trig[0] }, C, {0,0} };
		verts[ 2] = (SDL_Vertex){ { x + trig[6], y + trig[1] }, C, {0,0} };
		verts[ 3] = (SDL_Vertex){ { x + trig[5], y + trig[2] }, C, {0,0} };
		verts[ 4] = (SDL_Vertex){ { x + trig[4], y + trig[3] }, C, {0,0} };
		verts[ 5] = (SDL_Vertex){ { x + trig[3], y + trig[4] }, C, {0,0} };
		verts[ 6] = (SDL_Vertex){ { x + trig[2], y + trig[5] }, C, {0,0} };
		verts[ 7] = (SDL_Vertex){ { x + trig[1], y + trig[6] }, C, {0,0} };
		verts[ 8] = (SDL_Vertex){ { x + trig[0], y + trig[7] }, C, {0,0} };
		verts[ 9] = (SDL_Vertex){ { x          , y + rad     }, C, {0,0} };
		verts[10] = (SDL_Vertex){ { x - trig[0], y + trig[7] }, C, {0,0} };
		verts[11] = (SDL_Vertex){ { x - trig[1], y + trig[6] }, C, {0,0} };
		verts[12] = (SDL_Vertex){ { x - trig[2], y + trig[5] }, C, {0,0} };
		verts[13] = (SDL_Vertex){ { x - trig[3], y + trig[4] }, C, {0,0} };
		verts[14] = (SDL_Vertex){ { x - trig[4], y + trig[3] }, C, {0,0} };
		verts[15] = (SDL_Vertex){ { x - trig[5], y + trig[2] }, C, {0,0} };
		verts[16] = (SDL_Vertex){ { x - trig[6], y + trig[1] }, C, {0,0} };
		verts[17] = (SDL_Vertex){ { x - trig[7], y + trig[0] }, C, {0,0} };
		verts[18] = (SDL_Vertex){ { x - rad    , y           }, C, {0,0} };
		verts[19] = (SDL_Vertex){ { x - trig[7], y - trig[0] }, C, {0,0} };
		verts[20] = (SDL_Vertex){ { x - trig[6], y - trig[1] }, C, {0,0} };
		verts[21] = (SDL_Vertex){ { x - trig[5], y - trig[2] }, C, {0,0} };
		verts[22] = (SDL_Vertex){ { x - trig[4], y - trig[3] }, C, {0,0} };
		verts[23] = (SDL_Vertex){ { x - trig[3], y - trig[4] }, C, {0,0} };
		verts[24] = (SDL_Vertex){ { x - trig[2], y - trig[5] }, C, {0,0} };
		verts[25] = (SDL_Vertex){ { x - trig[1], y - trig[6] }, C, {0,0} };
		verts[26] = (SDL_Vertex){ { x - trig[0], y - trig[7] }, C, {0,0} };
		verts[27] = (SDL_Vertex){ { x          , y - rad     }, C, {0,0} };
		verts[28] = (SDL_Vertex){ { x + trig[0], y - trig[7] }, C, {0,0} };
		verts[29] = (SDL_Vertex){ { x + trig[1], y - trig[6] }, C, {0,0} };
		verts[30] = (SDL_Vertex){ { x + trig[2], y - trig[5] }, C, {0,0} };
		verts[31] = (SDL_Vertex){ { x + trig[3], y - trig[4] }, C, {0,0} };
		verts[32] = (SDL_Vertex){ { x + trig[4], y - trig[3] }, C, {0,0} };
		verts[33] = (SDL_Vertex){ { x + trig[5], y - trig[2] }, C, {0,0} };
		verts[34] = (SDL_Vertex){ { x + trig[6], y - trig[1] }, C, {0,0} };
		verts[35] = (SDL_Vertex){ { x + trig[7], y - trig[0] }, C, {0,0} };
		rad = C_radius - L_radius;
		trig[0] = 0.173648*rad; trig[1] = 0.342020*rad; trig[2] = 0.500000*rad; 
		trig[3] = 0.642788*rad; trig[4] = 0.766044*rad; trig[5] = 0.866025*rad; 
		trig[6] = 0.939693*rad; trig[7] = 0.984808*rad;
		verts[36] = (SDL_Vertex){ { x + rad    , y           }, C, {0,0} };
		verts[37] = (SDL_Vertex){ { x + trig[7], y + trig[0] }, C, {0,0} };
		verts[38] = (SDL_Vertex){ { x + trig[6], y + trig[1] }, C, {0,0} };
		verts[39] = (SDL_Vertex){ { x + trig[5], y + trig[2] }, C, {0,0} };
		verts[40] = (SDL_Vertex){ { x + trig[4], y + trig[3] }, C, {0,0} };
		verts[41] = (SDL_Vertex){ { x + trig[3], y + trig[4] }, C, {0,0} };
		verts[42] = (SDL_Vertex){ { x + trig[2], y + trig[5] }, C, {0,0} };
		verts[43] = (SDL_Vertex){ { x + trig[1], y + trig[6] }, C, {0,0} };
		verts[44] = (SDL_Vertex){ { x + trig[0], y + trig[7] }, C, {0,0} };
		verts[45] = (SDL_Vertex){ { x          , y + rad     }, C, {0,0} };
		verts[46] = (SDL_Vertex){ { x - trig[0], y + trig[7] }, C, {0,0} };
		verts[47] = (SDL_Vertex){ { x - trig[1], y + trig[6] }, C, {0,0} };
		verts[48] = (SDL_Vertex){ { x - trig[2], y + trig[5] }, C, {0,0} };
		verts[49] = (SDL_Vertex){ { x - trig[3], y + trig[4] }, C, {0,0} };
		verts[50] = (SDL_Vertex){ { x - trig[4], y + trig[3] }, C, {0,0} };
		verts[51] = (SDL_Vertex){ { x - trig[5], y + trig[2] }, C, {0,0} };
		verts[52] = (SDL_Vertex){ { x - trig[6], y + trig[1] }, C, {0,0} };
		verts[53] = (SDL_Vertex){ { x - trig[7], y + trig[0] }, C, {0,0} };
		verts[54] = (SDL_Vertex){ { x - rad    , y           }, C, {0,0} };
		verts[55] = (SDL_Vertex){ { x - trig[7], y - trig[0] }, C, {0,0} };
		verts[56] = (SDL_Vertex){ { x - trig[6], y - trig[1] }, C, {0,0} };
		verts[57] = (SDL_Vertex){ { x - trig[5], y - trig[2] }, C, {0,0} };
		verts[58] = (SDL_Vertex){ { x - trig[4], y - trig[3] }, C, {0,0} };
		verts[59] = (SDL_Vertex){ { x - trig[3], y - trig[4] }, C, {0,0} };
		verts[60] = (SDL_Vertex){ { x - trig[2], y - trig[5] }, C, {0,0} };
		verts[61] = (SDL_Vertex){ { x - trig[1], y - trig[6] }, C, {0,0} };
		verts[62] = (SDL_Vertex){ { x - trig[0], y - trig[7] }, C, {0,0} };
		verts[63] = (SDL_Vertex){ { x          , y - rad     }, C, {0,0} };
		verts[64] = (SDL_Vertex){ { x + trig[0], y - trig[7] }, C, {0,0} };
		verts[65] = (SDL_Vertex){ { x + trig[1], y - trig[6] }, C, {0,0} };
		verts[66] = (SDL_Vertex){ { x + trig[2], y - trig[5] }, C, {0,0} };
		verts[67] = (SDL_Vertex){ { x + trig[3], y - trig[4] }, C, {0,0} };
		verts[68] = (SDL_Vertex){ { x + trig[4], y - trig[3] }, C, {0,0} };
		verts[69] = (SDL_Vertex){ { x + trig[5], y - trig[2] }, C, {0,0} };
		verts[70] = (SDL_Vertex){ { x + trig[6], y - trig[1] }, C, {0,0} };
		verts[71] = (SDL_Vertex){ { x + trig[7], y - trig[0] }, C, {0,0} };
		int indices[216] = { 0, 1, 36, 1, 37, 36, 1, 2, 37, 2, 38, 37, 2, 3, 
			38, 3, 39, 38, 3, 4, 39, 4, 40, 39, 4, 5, 40, 5, 41, 40, 5, 6, 41,
			6, 42, 41, 6, 7, 42, 7, 43, 42, 7, 8, 43, 8, 44, 43, 8, 9, 44, 9, 
			45, 44, 9, 10, 45, 10, 46, 45, 10, 11, 46, 11, 47, 46, 11, 12, 47, 
			12, 48, 47, 12, 13, 48, 13, 49, 48, 13, 14, 49, 14, 50, 49, 14, 15, 
			50, 15, 51, 50, 15, 16, 51, 16, 52, 51, 16, 17, 52, 17, 53, 52, 17, 
			18, 53, 18, 54, 53, 18, 19, 54, 19, 55, 54, 19, 20, 55, 20, 56, 55, 
			20, 21, 56, 21, 57, 56, 21, 22, 57, 22, 58, 57, 22, 23, 58, 23, 59, 
			58, 23, 24, 59, 24, 60, 59, 24, 25, 60, 25, 61, 60, 25, 26, 61, 26, 
			62, 61, 26, 27, 62, 27, 63, 62, 27, 28, 63, 28, 64, 63, 28, 29, 64, 
			29, 65, 64, 29, 30, 65, 30, 66, 65, 30, 31, 66, 31, 67, 66, 31, 32, 
			67, 32, 68, 67, 32, 33, 68, 33, 69, 68, 33, 34, 69, 34, 70, 69, 34, 
			35, 70, 35, 71, 70, 35, 0, 71, 0, 36, 71 };
		SDL_RenderGeometry( R, NULL, verts, 72, indices, 216 );
}
void gp_fill_36circle(SDL_Renderer *R, int x, int y, int radius){
		SDL_Color C = SDL_GetRender_SDL_Color( R );
		const float trig [8] = { 0.173648*radius, 0.342020*radius, 0.500000*radius, 0.642788*radius, 0.766044*radius, 0.866025*radius, 0.939693*radius, 0.984808*radius};
		SDL_Vertex verts[36];
		verts[ 0] = (SDL_Vertex){ { x + radius , y           }, C, {0,0} };
		verts[ 1] = (SDL_Vertex){ { x + trig[7], y + trig[0] }, C, {0,0} };
		verts[ 2] = (SDL_Vertex){ { x + trig[6], y + trig[1] }, C, {0,0} };
		verts[ 3] = (SDL_Vertex){ { x + trig[5], y + trig[2] }, C, {0,0} };
		verts[ 4] = (SDL_Vertex){ { x + trig[4], y + trig[3] }, C, {0,0} };
		verts[ 5] = (SDL_Vertex){ { x + trig[3], y + trig[4] }, C, {0,0} };
		verts[ 6] = (SDL_Vertex){ { x + trig[2], y + trig[5] }, C, {0,0} };
		verts[ 7] = (SDL_Vertex){ { x + trig[1], y + trig[6] }, C, {0,0} };
		verts[ 8] = (SDL_Vertex){ { x + trig[0], y + trig[7] }, C, {0,0} };
		verts[ 9] = (SDL_Vertex){ { x          , y + radius  }, C, {0,0} };
		verts[10] = (SDL_Vertex){ { x - trig[0], y + trig[7] }, C, {0,0} };
		verts[11] = (SDL_Vertex){ { x - trig[1], y + trig[6] }, C, {0,0} };
		verts[12] = (SDL_Vertex){ { x - trig[2], y + trig[5] }, C, {0,0} };
		verts[13] = (SDL_Vertex){ { x - trig[3], y + trig[4] }, C, {0,0} };
		verts[14] = (SDL_Vertex){ { x - trig[4], y + trig[3] }, C, {0,0} };
		verts[15] = (SDL_Vertex){ { x - trig[5], y + trig[2] }, C, {0,0} };
		verts[16] = (SDL_Vertex){ { x - trig[6], y + trig[1] }, C, {0,0} };
		verts[17] = (SDL_Vertex){ { x - trig[7], y + trig[0] }, C, {0,0} };
		verts[18] = (SDL_Vertex){ { x - radius , y           }, C, {0,0} };
		verts[19] = (SDL_Vertex){ { x - trig[7], y - trig[0] }, C, {0,0} };
		verts[20] = (SDL_Vertex){ { x - trig[6], y - trig[1] }, C, {0,0} };
		verts[21] = (SDL_Vertex){ { x - trig[5], y - trig[2] }, C, {0,0} };
		verts[22] = (SDL_Vertex){ { x - trig[4], y - trig[3] }, C, {0,0} };
		verts[23] = (SDL_Vertex){ { x - trig[3], y - trig[4] }, C, {0,0} };
		verts[24] = (SDL_Vertex){ { x - trig[2], y - trig[5] }, C, {0,0} };
		verts[25] = (SDL_Vertex){ { x - trig[1], y - trig[6] }, C, {0,0} };
		verts[26] = (SDL_Vertex){ { x - trig[0], y - trig[7] }, C, {0,0} };
		verts[27] = (SDL_Vertex){ { x          , y - radius  }, C, {0,0} };
		verts[28] = (SDL_Vertex){ { x + trig[0], y - trig[7] }, C, {0,0} };
		verts[29] = (SDL_Vertex){ { x + trig[1], y - trig[6] }, C, {0,0} };
		verts[30] = (SDL_Vertex){ { x + trig[2], y - trig[5] }, C, {0,0} };
		verts[31] = (SDL_Vertex){ { x + trig[3], y - trig[4] }, C, {0,0} };
		verts[32] = (SDL_Vertex){ { x + trig[4], y - trig[3] }, C, {0,0} };
		verts[33] = (SDL_Vertex){ { x + trig[5], y - trig[2] }, C, {0,0} };
		verts[34] = (SDL_Vertex){ { x + trig[6], y - trig[1] }, C, {0,0} };
		verts[35] = (SDL_Vertex){ { x + trig[7], y - trig[0] }, C, {0,0} };
		int indices[102] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 
			6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 11, 0, 11, 12, 0, 12, 13,
			0, 13, 14, 0, 14, 15, 0, 15, 16, 0, 16, 17, 0, 17, 18, 0, 18, 19, 
			0, 19, 20, 0, 20, 21, 0, 21, 22, 0, 22, 23, 0, 23, 24, 0, 24, 25, 
			0, 25, 26, 0, 26, 27, 0, 27, 28, 0, 28, 29, 0, 29, 30, 0, 30, 31, 
			0, 31, 32, 0, 32, 33, 0, 33, 34, 0, 34, 35 };
		SDL_RenderGeometry( R, NULL, verts, 36, indices, 102 );
}

void gp_draw_fastcircle(SDL_Renderer *R, int x, int y, int radius){
	if( radius < circle_threshold8 ){
		gp_draw_8circle( R, x, y, radius );
	}
	else if( radius < circle_threshold16 ){
		gp_draw_16circle( R, x, y, radius );
	}
	else if( radius < circle_threshold24 ){
		gp_draw_24circle( R, x, y, radius );
	}
	else {
		gp_draw_36circle( R, x, y, radius );
	}
}
void gp_drawthick_fastcircle(SDL_Renderer *R, int x, int y, int C_radius, float L_radius ){
	if( C_radius < circle_threshold8 ){
		gp_drawthick_8circle( R, x, y, C_radius, L_radius );
	}
	else if( C_radius < circle_threshold16 ){
		gp_drawthick_16circle( R, x, y, C_radius, L_radius );
	}
	else if( C_radius < circle_threshold24 ){
		gp_drawthick_24circle( R, x, y, C_radius, L_radius );
	}
	else {
		gp_drawthick_36circle( R, x, y, C_radius, L_radius );
	}
}
void gp_fill_fastcircle(SDL_Renderer *R, int x, int y, int radius){
	if( radius < circle_threshold8 ){
		gp_fill_8circle( R, x, y, radius );
	}
	else if( radius < circle_threshold16 ){
		gp_fill_16circle( R, x, y, radius );
	}
	else if( radius < circle_threshold24 ){
		gp_fill_24circle( R, x, y, radius );
	}
	else {
		gp_fill_36circle( R, x, y, radius );
	}  
}

/*
	int x = 100;
	clock_t t8 = clock();
	for (int i = 0; i < 1000; ++i ){
		gp_fill_8circle( renderer, x + i, 100+i, i );
		x += 1;
	}
	x = 100;
	clock_t t10 = clock();
	for (int i = 0; i < 1000; ++i ){
		gp_fill_10circle( renderer, x + i, 200+i, i );
		x += 1;
	}
	x = 100;
	clock_t t12 = clock();
	for (int i = 0; i < 1000; ++i ){
		gp_fill_12circle( renderer, x + i, 300+i, i );
		x += 1;
	}
	x = 100;
	clock_t t16 = clock();
	for (int i = 0; i < 1000; ++i ){
		gp_fill_16circle( renderer, x + i, 400+i, i );
		x += 1;
	}
	x = 100;
	clock_t tperf = clock();
	for (int i = 0; i < 1000; ++i ){
		gp_fill_circle( renderer, x + i, 500+i, i );
		x += 1;
	}
	clock_t tfinal = clock();

	printf("%d, %d, %d, %d, %d, %d\n", t8, t10, t12, t16, tperf, tfinal );
	printf("%d, %d, %d, %d, %d\n", t10-t8, t12-t10, t16-t12, tperf-t16, tfinal-tperf );
	//RESULTS
	//258, 260, 261, 264, 269, 789
	//2, 1, 3, 5, 520
*/


//9-40-114
//6-28-78
//3-16-42
#define rounded_res 6 //resolution
#define rounded_len 28// 4*(res + 1)
#define rounded_tri 78// 3 * len -6

void gp_draw_roundedRect( SDL_Renderer *R, SDL_Rect *rect, int radius ){


}


void gp_fill_roundedRect( SDL_Renderer *R, SDL_Rect *rect, int radius ){

	if( 2*radius > rect->w ) radius = rect->w / 2;
	if( 2*radius > rect->h ) radius = rect->h / 2;

	SDL_Color C = SDL_GetRender_SDL_Color( R );

	SDL_Vertex verts[ rounded_len ];
	double theta = HALF_PI / rounded_res;

	float  xpr = rect->x + radius;
	float xwmr = rect->x + rect->w - radius;
	float  ypr = rect->y + radius;
	float yhmr = rect->y + rect->h - radius;

	for(int i = 0; i <= rounded_res; i++){
		
		double a = i * theta;
		double cosi = cos( a ) * radius;
		double sine = sin( a ) * radius;

		verts[  rounded_res   - i] = (SDL_Vertex){ { xwmr + cosi, ypr  - sine }, C, {0,0} };
		verts[  rounded_res+1 + i] = (SDL_Vertex){ { xwmr + cosi, yhmr + sine }, C, {0,0} };
		verts[3*rounded_res+2 - i] = (SDL_Vertex){ { xpr  - cosi, yhmr + sine }, C, {0,0} };
		verts[3*rounded_res+3 + i] = (SDL_Vertex){ { xpr  - cosi, ypr  - sine }, C, {0,0} };
	}

	int indices [ rounded_tri ];
	for(int i = 0; i < rounded_len-2; i++){
		indices[ 3*i   ] = 0;
		indices[ 3*i+1 ] = i+1;
		indices[ 3*i+2 ] = i+2;
	}
	//int out = 
	SDL_RenderGeometry(R, NULL, verts, rounded_len, indices, rounded_tri);

	//if( out < 0 ){
	//    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_RenderGeometry: %s", SDL_GetError());
	//}

	//return out;
}


void gp_draw_thickRoundedPoly( SDL_Renderer *R, vec2d *verts, int verts_count, float radius ){
	for (int i = 0; i < verts_count-1; ++i ){
		gp_draw_thickLine( R, verts[i].x, verts[i].y, verts[i+1].x, verts[i+1].y, radius );
		gp_fill_fastcircle( R, verts[i].x, verts[i].y, radius );
	}
	int i = verts_count-1;
	gp_draw_thickLine( R, verts[i].x, verts[i].y, verts[0].x, verts[0].y, radius );
	gp_fill_fastcircle( R, verts[i].x, verts[i].y, radius );
}



void gp_fill_poly( SDL_Renderer *R, vec2d *verts, int verts_count ){
	SDL_Color C = SDL_GetRender_SDL_Color( R );
	SDL_Vertex *sdlverts = malloc( verts_count * sizeof(SDL_Vertex) );
	for (int i = 0; i < verts_count; ++i ){
		 sdlverts[i] = (SDL_Vertex){ { verts[i].x, verts[i].y }, C, {0,0} };
	}
	int N = 3*(verts_count-2);
	int *indices = malloc( N * sizeof(int) );
	for(int i = 0; i < verts_count-2; i++){
		indices[ 3*i   ] = 0;
		indices[ 3*i+1 ] = i+1;
		indices[ 3*i+2 ] = i+2;
	}
	SDL_RenderGeometry( R, NULL, sdlverts, verts_count, indices, N );
	free( sdlverts );
	free( indices );
}