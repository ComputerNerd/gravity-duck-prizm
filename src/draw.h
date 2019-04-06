#ifndef _DRAW
#define _DRAW

#include "level.h"

#define SCREEN_WIDTH    320 //custom size
#define SCREEN_HEIGHT   216
#define TILE_WIDTH      16
#define TILE_HEIGHT     16
#define MASK            0xFFFF


typedef struct {
	int x, y;
} Camera;

int draw_init(void);
void draw_quit(void);
void draw_tileMap(const Level* level, Camera camera);
void draw_objects(const Level* level, Camera camera);
void draw_background(void);
void draw_tipsPanel(void);

#endif //DRAW
