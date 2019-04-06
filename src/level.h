#ifndef _LEVEL
#define _LEVEL

#include "object.h"

#define NB_LEVEL 40

typedef enum {
	OUT=-1, EMPTY, GROUND, BEGIN, // -1 ~ 2
	EGG_UP, EGG_DOWN, EGG_LEFT, EGG_RIGHT, // 3 ~ 6
	SPIKE_UP, SPIKE_DOWN, SPIKE_LEFT, SPIKE_RIGHT, // 7 ~ 10
	GRAVITY_PLUS, GRAVITY_MINUS, // 11 ~ 12
	HORIZONTAL_CUBE, VERTICAL_CUBE, // 13 ~ 14
	HORIZONTAL_CATERPILLAR, VERTICAL_CATERPILLAR, // 15 ~ 16
	PLANT_LEFT_1, PLANT_RIGHT_1, // 17 ~ 18
	PLANT_LEFT_2, PLANT_RIGHT_2, // 19 ~ 20
	BEE_1, BEE_2, // 21 ~ 22
	PESTLE_UP_1, PESTLE_DOWN_1, PESTLE_LEFT_1, PESTLE_RIGHT_1, // 23 ~ 26
	PESTLE_UP_2, PESTLE_DOWN_2, PESTLE_LEFT_2, PESTLE_RIGHT_2 // 27 ~ 30
} Map_item;

typedef enum {
	G_OUT=11, G_NORMAL, //G_OUT = SPIKE_RIGHT+1
	G_UDLR, G_DLR, G_LR, G_DL, G_DR, G_D, G_L, G_R,
	G_GRASS_UDL, G_GRASS_UDR, G_GRASS_ULR, G_GRASS_UD, G_GRASS_UL, G_GRASS_UR, G_GRASS_U,
	G_EGG_UP, G_EGG_DOWN, G_EGG_LEFT, G_EGG_RIGHT,
	G_ANGLE_UL, G_ANGLE_UR, G_ANGLE_DL, G_ANGLE_DR
} Ground_tile;

typedef struct {
	short id, width, height;
	char* map;
	Direction gravity;
	Object** object;
	char* tips;
} Level;

Level* level_load(int n);
void level_free(Level* level);

#endif //_LEVEL
