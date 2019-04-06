#include <stdlib.h>
#include <string.h>

#include "tileset.h"
#include "graphic_functions.h"

#ifndef NULL
#define	NULL	0
#endif

static Tileset* tileset_create(const char* bmp, int width, int height, int length, const short* palette, int palette_length);
static void tileset_horizontal_symmetry(Tileset* tileset);
static void tileset_vertical_symmetry(Tileset* tileset);
static void tileset_rotation_90_degrees(Tileset* tileset);

#include "bitmaps.h"

/* public functions */
Tileset* tileset_load(Tileset_name name)
{
	Tileset* tileset = NULL;
	if(name == TITLE) {
		tileset = tileset_create(bmp_title, 240, 112, 1, palette_title, 18);
	} else if(name == SELECT_LEVEL) {
		tileset = tileset_create(bmp_select_level, 224, 48, 1, palette_select_level, 9);
	} else if(name == SELECT_LEVEL_BG) {
		tileset = tileset_create(bmp_select_level_bg, 192, 128, 1, palette_select_level_bg, 8);
	} else if(name == BUTTON) {
		tileset = tileset_create(bmp_button, 20, 20, 3, palette_button, 15);
	} else if(name == PAUSE) {
		tileset = tileset_create(bmp_pause, 80, 88, 1, palette_pause, 10);
	} else if(name == PAUSE_BUTTON) {
		tileset = tileset_create(bmp_pause_button, 66, 22, 1, palette_pause_button, 6);
	} else if(name == STATUE) {
		tileset = tileset_create(bmp_statue, 172, 132, 1, palette_statue, 46);
	} else if(name == DUCK_INTRO) {
		tileset = tileset_create(bmp_duck_intro, 20, 21, 5, palette_duck_intro, 23);
	} else if(name == CONGRATULATIONS) {
		tileset = tileset_create(bmp_congratulations, 310, 32, 1, palette_congratulations, 2);
	} else if(name == GAME_COMPLETE) {
		tileset = tileset_create(bmp_game_complete, 152, 16, 1, palette_game_complete, 2);
	} else if(name == HAT_DUCK) {
		tileset = tileset_create(bmp_hat_duck, 39, 69, 1, palette_hat_duck, 14);
	} else if(name == END_BUTTON) {
		tileset = tileset_create(bmp_end_button, 96, 24, 1, palette_end_button, 3);
	} else if(name == T_GROUND) {
		tileset = tileset_create(bmp_ground, 16, 16, 25, palette_ground, 23);
	} else if(name == T_SPIKE_UP) {
		tileset = tileset_create(bmp_spike_up, 16, 16, 1, palette_spike_up, 8);
	} else if(name == T_SPIKE_RIGHT) {
		tileset = tileset_load(T_SPIKE_UP);
		tileset_rotation_90_degrees(tileset);
	} else if(name == T_SPIKE_DOWN) {
		tileset = tileset_load(T_SPIKE_RIGHT);
		tileset_rotation_90_degrees(tileset);
	} else if(name == T_SPIKE_LEFT) {
		tileset = tileset_load(T_SPIKE_DOWN);
		tileset_rotation_90_degrees(tileset);
	} else if(name == T_EGG_UP) {
		tileset = tileset_create(bmp_egg_up, 16, 16, 16, palette_egg_up, 15);
	} else if(name == T_EGG_RIGHT) {
		tileset = tileset_load(T_EGG_UP);
		tileset_rotation_90_degrees(tileset);
	} else if(name == T_EGG_DOWN) {
		tileset = tileset_load(T_EGG_RIGHT);
		tileset_rotation_90_degrees(tileset);
	} else if(name == T_EGG_LEFT) {
		tileset = tileset_load(T_EGG_DOWN);
		tileset_rotation_90_degrees(tileset);
	} else if(name == EGG_CAUGHT) {
		tileset = tileset_create(bmp_egg_caught, 16, 16, 4, palette_egg_caught, 2);
	} else if(name == DUCK_UP_RIGHT) {
		tileset = tileset_create(bmp_duck_right, 16, 16, 9, palette_duck_right, 10);
	} else if(name == DUCK_UP_LEFT) {
		tileset = tileset_load(DUCK_UP_RIGHT);
		tileset_horizontal_symmetry(tileset);
	} else if(name == DUCK_DOWN_RIGHT) {
		tileset = tileset_load(DUCK_UP_RIGHT);
		tileset_vertical_symmetry(tileset);
	} else if(name == DUCK_DOWN_LEFT) {
		tileset = tileset_load(DUCK_DOWN_RIGHT);
		tileset_horizontal_symmetry(tileset);
	} else if(name == DUCK_RIGHT_UP) {
		tileset = tileset_load(DUCK_RIGHT_DOWN);
		tileset_vertical_symmetry(tileset);
	} else if(name == DUCK_RIGHT_DOWN) {
		tileset = tileset_load(DUCK_UP_RIGHT);
		tileset_rotation_90_degrees(tileset);
	} else if(name == DUCK_LEFT_UP) {
		tileset = tileset_load(DUCK_RIGHT_UP);
		tileset_horizontal_symmetry(tileset);
	} else if(name == DUCK_LEFT_DOWN) {
		tileset = tileset_load(DUCK_RIGHT_DOWN);
		tileset_horizontal_symmetry(tileset);
	} /*else if(name == GROWING_DUCK_UP_RIGHT) {
		const char bmp[] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
			0,0,0,0,0,0,0,1,2,1,0,0,0,0,0,0,
			0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
			0,0,0,0,0,0,1,3,3,2,1,0,0,0,0,0,
			0,0,0,0,0,0,1,3,2,2,1,0,0,0,0,0,
			0,0,0,0,0,0,1,3,3,2,1,0,0,0,0,0,
			0,0,0,0,0,0,0,1,1,4,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,
			0,0,0,0,0,0,1,5,5,3,2,1,0,0,0,0,
			0,0,0,0,0,1,5,5,3,2,2,1,0,0,0,0,
			0,0,0,0,0,1,5,3,3,2,2,1,0,0,0,0,
			0,0,0,0,0,0,1,5,3,3,2,1,0,0,0,0,
			0,0,0,0,0,0,0,1,1,4,4,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
			0,0,0,0,0,1,5,5,3,2,2,1,0,0,0,0,
			0,0,0,0,0,1,5,3,2,2,2,1,0,0,0,0,
			0,0,0,0,1,5,5,3,2,2,2,1,0,0,0,0,
			0,0,0,0,1,5,3,3,2,2,2,1,0,0,0,0,
			0,0,0,0,1,5,3,3,2,2,2,1,0,0,0,0,
			0,0,0,0,0,1,5,3,3,2,2,1,0,0,0,0,
			0,0,0,0,0,0,1,1,4,4,4,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
			0,0,0,0,1,5,5,3,3,2,2,1,0,0,0,0,
			0,0,0,0,1,5,3,4,4,4,4,1,0,0,0,0,
			0,0,0,0,1,5,4,6,7,7,7,1,0,0,0,0,
			0,0,0,0,1,5,3,4,4,4,4,1,0,0,0,0,
			0,0,0,1,5,5,3,3,2,2,2,1,0,0,0,0,
			0,0,0,1,5,3,3,2,2,2,2,1,0,0,0,0,
			0,0,0,1,5,3,3,3,2,2,2,1,0,0,0,0,
			0,0,0,0,1,5,3,3,3,2,2,1,0,0,0,0,
			0,0,0,0,0,1,1,1,4,4,4,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
			0,0,0,0,1,5,5,3,3,2,2,1,0,0,0,0,
			0,0,0,0,1,5,3,4,4,4,4,4,0,0,0,0,
			0,0,0,0,1,5,4,6,7,7,7,7,4,0,0,0,
			0,0,0,0,1,5,4,6,6,6,6,6,4,0,0,0,
			0,0,0,0,1,5,3,4,4,4,4,4,0,0,0,0,
			0,0,0,1,5,5,3,3,2,2,2,1,0,0,0,0,
			0,0,0,1,5,3,1,2,2,2,2,1,0,0,0,0,
			0,0,0,1,1,1,3,3,2,2,2,1,0,0,0,0,
			0,0,0,0,1,5,3,3,3,2,2,1,0,0,0,0,
			0,0,0,0,0,1,1,1,4,4,4,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
			0,0,0,0,1,5,5,3,3,2,2,1,0,0,0,0,
			0,0,0,0,1,5,8,3,2,2,8,1,0,0,0,0,
			0,0,0,0,1,5,3,4,4,4,4,4,4,0,0,0,
			0,0,0,0,1,5,4,6,7,7,7,7,7,4,0,0,
			0,0,0,0,1,5,4,6,6,6,6,6,6,4,0,0,
			0,0,0,1,5,5,3,4,4,4,4,4,4,0,0,0,
			0,0,0,1,5,3,3,3,3,3,3,4,0,0,0,0,
			0,0,1,5,5,1,3,3,2,2,2,2,1,0,0,0,
			0,0,1,5,3,1,3,2,2,2,2,2,1,0,0,0,
			0,0,1,1,1,5,3,3,2,2,2,2,1,0,0,0,
			0,0,1,1,5,3,1,3,3,2,2,4,1,0,0,0,
			0,0,0,4,9,7,4,1,4,9,7,4,0,0,0,0,
			0,0,0,0,4,4,0,0,0,4,4,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		};
		const short palette[] = {65535,18816,65325,65024,18688,52320,47840,64480,1016,43680};
		//do_nothing_with(palette[0]);
		tileset = tileset_create(bmp, 16, 16, 7, palette, 10);
	} else if(name == GROWING_DUCK_UP_LEFT) {
		tileset = tileset_load(GROWING_DUCK_UP_RIGHT);
		tileset_horizontal_symmetry(tileset);
	} else if(name == GROWING_DUCK_DOWN_RIGHT) {
		tileset = tileset_load(GROWING_DUCK_UP_RIGHT);
		tileset_vertical_symmetry(tileset);
	} else if(name == GROWING_DUCK_DOWN_LEFT) {
		tileset = tileset_load(GROWING_DUCK_DOWN_RIGHT);
		tileset_horizontal_symmetry(tileset);
	} else if(name == GROWING_DUCK_RIGHT_UP) {
		tileset = tileset_load(GROWING_DUCK_RIGHT_DOWN);
		tileset_vertical_symmetry(tileset);
	} else if(name == GROWING_DUCK_RIGHT_DOWN) {
		tileset = tileset_load(GROWING_DUCK_UP_RIGHT);
		tileset_rotation_90_degrees(tileset);
	} else if(name == GROWING_DUCK_LEFT_UP) {
		tileset = tileset_load(GROWING_DUCK_RIGHT_UP);
		tileset_horizontal_symmetry(tileset);
	} else if(name == GROWING_DUCK_LEFT_DOWN) {
		tileset = tileset_load(GROWING_DUCK_RIGHT_DOWN);
		tileset_horizontal_symmetry(tileset);
	}*/ else if(name == T_GRAVITY_SWITCH) {
		tileset = tileset_create(bmp_gravity_switch, 16, 16, 8, palette_gravity_switch, 2);
	} else if(name == T_CUBE) {
		tileset = tileset_create(bmp_cube, 16, 16, 1, palette_cube, 11);
	} else if(name == CATERPILLAR_UP_RIGHT) {
		tileset = tileset_create(bmp_caterpillar_up_right, 16, 16, 6, palette_caterpillar_up_right, 6);
	} else if(name == CATERPILLAR_UP_LEFT) {
		tileset = tileset_load(CATERPILLAR_UP_RIGHT);
		tileset_horizontal_symmetry(tileset);
	} else if(name == CATERPILLAR_DOWN_RIGHT) {
		tileset = tileset_load(CATERPILLAR_UP_RIGHT);
		tileset_vertical_symmetry(tileset);
	} else if(name == CATERPILLAR_DOWN_LEFT) {
		tileset = tileset_load(CATERPILLAR_DOWN_RIGHT);
		tileset_horizontal_symmetry(tileset);
	} else if(name == CATERPILLAR_RIGHT_UP) {
		tileset = tileset_load(CATERPILLAR_RIGHT_DOWN);
		tileset_vertical_symmetry(tileset);
	} else if(name == CATERPILLAR_RIGHT_DOWN) {
		tileset = tileset_load(CATERPILLAR_UP_RIGHT);
		tileset_rotation_90_degrees(tileset);
	} else if(name == CATERPILLAR_LEFT_UP) {
		tileset = tileset_load(CATERPILLAR_RIGHT_UP);
		tileset_horizontal_symmetry(tileset);
	} else if(name == CATERPILLAR_LEFT_DOWN) {
		tileset = tileset_load(CATERPILLAR_RIGHT_DOWN);
		tileset_horizontal_symmetry(tileset);
	} else if(name == T_PLANT_RIGHT) {
		tileset = tileset_create(bmp_plant_right, 16, 16, 3, palette_plant_right, 8);
	} else if(name == T_PLANT_LEFT) {
		tileset = tileset_load(T_PLANT_RIGHT);
		tileset_horizontal_symmetry(tileset);
	} else if(name == T_BEE_RIGHT) {
		tileset = tileset_create(bmp_bee_right, 16, 16, 4, palette_bee_right, 10);
	} else if(name == T_BEE_LEFT) {
		tileset = tileset_load(T_BEE_RIGHT);
		tileset_horizontal_symmetry(tileset);
	} else if(name == BEE_HALO) {
		tileset = tileset_create(bmp_bee_halo, 16, 16, 6, palette_bee_halo, 2);
	} else if(name == T_BULLET) {
		tileset = tileset_create(bmp_bullet, 4, 4, 2, palette_bullet, 5);
	} else if(name == T_PESTLE_UP) {
		tileset = tileset_create(bmp_pestle_up, 16, 32, 6, palette_pestle_up, 12);
	} else if(name == T_PESTLE_DOWN) {
		tileset = tileset_load(T_PESTLE_RIGHT);
		tileset_rotation_90_degrees(tileset);
	} else if(name == T_PESTLE_RIGHT) {
		tileset = tileset_load(T_PESTLE_UP);
		tileset_rotation_90_degrees(tileset);
	} else if(name == T_PESTLE_LEFT) {
		tileset = tileset_load(T_PESTLE_DOWN);
		tileset_rotation_90_degrees(tileset);
	} else if(name == BIG_BLAST) {
		tileset = tileset_create(bmp_big_blast, 24, 24, 1, palette_big_blast, 2);
	} else if(name == SMALL_BLAST) {
		tileset = tileset_create(bmp_small_blast, 12, 12, 1, palette_small_blast, 2);
	}
	return tileset;
}

void tileset_free(Tileset* tileset)
{
	int i;
	if(tileset) {
		if(tileset->tile) {
			for(i=0 ; i<tileset->length ; i++)
				free(tileset->tile[i]);
			free(tileset->tile);
		}
		free(tileset->palette);
		free(tileset);
	}
}

/* private functions */
static Tileset* tileset_create(const char* bmp, int width, int height, int length, const short* palette, int palette_length)
{
	Tileset* tileset;
	int i, byte_width, tile_size;
	if(!bmp) return NULL;
	byte_width = width*2;
/* Don't need to allocate 2 bytes per pixel to stored indexed data (Lephe) */
//	tile_szie = byte_width * height;
	tile_size = width * height;

	tileset = malloc(sizeof(Tileset));
	if(!tileset) return NULL;

	tileset->width = width;
	tileset->height = height;
	tileset->length = length;
	tileset->tile = malloc(sizeof(char*) * length);
	if(!tileset->tile) {
		free(tileset);
		return NULL;
	}

	for(i=0 ; i<length ; i++) {
		tileset->tile[i] = malloc(tile_size);
		if(!tileset->tile[i]) {
			for( ; i ; i--)
				free(tileset->tile[i-1]);
			free(tileset->tile);
			free(tileset);
			return NULL;
		}
		memcpy(tileset->tile[i], bmp+i*width*height, tile_size);
	}

	tileset->palette = malloc(palette_length*sizeof(short));
	if(!tileset->palette) {
		tileset_free(tileset);
		return NULL;
	}
	memcpy(tileset->palette, palette, palette_length*sizeof(short));

	return tileset;
}

static void tileset_horizontal_symmetry(Tileset* tileset)
{
	int byte_width, i, x, y;
	unsigned char* line;
	if(!tileset) return;
	byte_width = tileset->width;
	line = malloc(byte_width);
	if(!line) return;
	for(i=0 ; i<tileset->length ; i++) {
		for(y=0 ; y<tileset->height ; y++) {
			for(x=0 ; x<tileset->width ; x++)
				line[tileset->width-x-1] = tileset->tile[i][y*tileset->width+x];
			memcpy(tileset->tile[i]+y*tileset->width, line, byte_width);
		}
	}
	free(line);
}

static void tileset_vertical_symmetry(Tileset* tileset)
{
	int byte_width, i, y;
	unsigned char* line;
	if(!tileset) return;
	byte_width = tileset->width;
	line = malloc(byte_width);
	if(!line) return;
	for(i=0 ; i<tileset->length ; i++) {
		for(y=0 ; y<tileset->height/2 ; y++) {
			memcpy(line, tileset->tile[i]+y*tileset->width, byte_width);
			memcpy(tileset->tile[i]+y*tileset->width, tileset->tile[i]+(tileset->height-1-y)*tileset->width, byte_width);
			memcpy(tileset->tile[i]+(tileset->height-1-y)*tileset->width, line, byte_width);
		}
	}
	free(line);
}

static void tileset_rotation_90_degrees(Tileset* tileset)
{
	int new_byte_width, new_tile_size, i, x, y;
	char* tmp_tile;
	if(!tileset) return;
	new_byte_width = tileset->height;
	new_tile_size = new_byte_width * tileset->width;
	for(i=0 ; i<tileset->length ; i++) {
		tmp_tile = malloc(new_tile_size);
		if(tmp_tile) {
			for(y=0 ; y<tileset->height ; y++) {
				for(x=0 ; x<tileset->width ; x++)
					tmp_tile[(x+1)*tileset->height-y-1] = tileset->tile[i][y*tileset->width+x];
			}
			free(tileset->tile[i]);
			tileset->tile[i] = tmp_tile;
		}
	}
	i = tileset->width;
	tileset->width = tileset->height;
	tileset->height = i;
}
