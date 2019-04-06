#include <stdlib.h>
#include <fxcg/display.h>
#include "graphic_functions.h"

#include "draw.h"
#include "intro.h"
#include "key.h"
#include "text.h"
#include "tileset.h"
#include "time.h"

#define NB_CONFETTI 30
#define sgn(x) (x<0?-1:x>0?1:0)

typedef struct {
	int x, y, ax, dx, ddx, ay, dy, color;
} Confetti;

static void intro_main(int end);
static void intro_congratulations();
static int intro_timer(int id, int delay);
static void intro_background(int end);

void *memcpy(void * restrict src, const void * restrict dst, size_t size);

/* public functions */
void intro() {
	intro_main(0);
}

void ending() {
	intro_main(1);
}

/* private functions */
static void intro_main(int end)
{
	int page, i, j=0;
	char str[51];
	const char *dialog[] = {
		"\"HELLO THERE MY LITTLE MINION.\"",
		"\"BRING ME 40 GOLDEN EGGS AND\nI WILL MAKE YOU RICH!\"",
		"\"YOU NOW HAVE THE POWER TO\nCONTROL GRAVITY.\"",
		"\"DO NOT FAIL ME... NOW GO!\"",
		"\"AH, GOOD, I SEE YOU HAVE\nRETURNED WITH THE EGGS!\"",
		"\"HERE. THIS IS YOUR REWARD.\"",
		"\"NOW BEGONE! I HAVE AN OMELET\nTO MAKE!\""
	}, *tips[] = {
		"PRESS EXIT TO SKIP",
		"(PRESS SHIFT)"
	};
	Tileset *statue, *duck;
	statue = tileset_load(STATUE);
	duck = tileset_load(DUCK_INTRO);
	if(statue && duck) {
		for(page=(end?4:0) ; page<(end?7:4) ; page++) {
			intro_background(0);
			text_print((LCD_WIDTH_PX-text_width(tips[0]))/2, 200, tips[0], 1, MASK);
			i = 0;
			while(dialog[page][i]) {
				if(intro_timer(0, 5)) i++;
				memcpy(str, dialog[page], i+1); str[i+1] = '\0';
				CopySpriteMasked(statue->tile[0], statue->palette, (SCREEN_WIDTH-statue->width)/2, 25, statue->width, statue->height, MASK);
				if(page == 2) {
					if(j<6 && intro_timer(1, 16)) j++;
					CopySpriteMasked(duck->tile[4-abs(j-3)], duck->palette, (SCREEN_WIDTH-statue->width)/2+128, 97, duck->width, duck->height, MASK);
				} else {
					if(page == 5) {
						if(j<70 && intro_timer(1, 4)) j += 5;
						CopySpriteMasked(duck->tile[0], duck->palette, (SCREEN_WIDTH-statue->width)/2+87, 32+j, duck->width, duck->height, MASK);
					} else  if(page > 5) CopySpriteMasked(duck->tile[0], duck->palette, (SCREEN_WIDTH-statue->width)/2+87, 102, duck->width, duck->height, MASK);
					CopySpriteMasked(duck->tile[1], duck->palette, (SCREEN_WIDTH-statue->width)/2+128, 97, duck->width, duck->height, MASK);
				}
				text_print(76+32, 163, str, 1, MASK);
				Bdisp_PutDisp_DD();
				if(key_down(K_SHIFT) || key_down(K_EXIT)) break;
			}
			CopySpriteMasked(statue->tile[0], statue->palette, (SCREEN_WIDTH-statue->width)/2, 25, statue->width, statue->height, MASK);
			CopySpriteMasked(duck->tile[1], duck->palette, (SCREEN_WIDTH-statue->width)/2+128, 97, duck->width, duck->height, MASK);
			if(page >= 5) CopySpriteMasked(duck->tile[0], duck->palette, (SCREEN_WIDTH-statue->width)/2+87, 102, duck->width, duck->height, MASK);
			text_print(76+32, 163, dialog[page], 1, MASK);
			text_print((LCD_WIDTH_PX-text_width(tips[1]))/2, 183, tips[1], 1, MASK);
			Bdisp_PutDisp_DD();
			while(key_down(K_SHIFT));
			while(!key_down(K_SHIFT) && !key_down(K_EXIT));
			while(key_down(K_SHIFT));
			if(key_down(K_EXIT)) break;
		}
		while(key_down(K_EXIT));
		while(key_down(K_SHIFT));
	}
	tileset_free(statue);
	tileset_free(duck);
	if(end) intro_congratulations();
}

static void intro_congratulations()
{
	const short palette[] = {65184,64105,30560,43007}, confetti_color[] = {49151,65175,7584,26601,64617,65376,64105};
	int color=0, i;
	Confetti confetti[NB_CONFETTI];
	Tileset *congratulations, *game_complete, *hat_duck, *button;
	congratulations = tileset_load(CONGRATULATIONS);
	game_complete = tileset_load(GAME_COMPLETE);
	hat_duck = tileset_load(HAT_DUCK);
	button = tileset_load(END_BUTTON);

	if(congratulations && game_complete && hat_duck && button) {
		for(i=0 ; i<NB_CONFETTI ; i++) {
			confetti[i].ax = confetti[i].ay = 0;
			confetti[i].x = rand()%SCREEN_WIDTH;
			confetti[i].y = rand()%SCREEN_HEIGHT - SCREEN_HEIGHT;
			confetti[i].dy = rand()%4 + 1;
			confetti[i].dx = rand()%7-3;
			confetti[i].ddx = (rand()%2)*2-1;
			confetti[i].color = rand()%7;
		}
		while(!key_down(K_EXE) && !key_down(K_EXIT)) {
			if(intro_timer(0, 122)) congratulations->palette[0] = palette[(color=(color+1)%4)]; // "CONGRATULATIONS" changes color ~every second
			if(intro_timer(1, 1)) {
				for(i=0 ; i<NB_CONFETTI ; i++) {
					confetti[i].ay += confetti[i].dy;
					if(confetti[i].ay >= 4) { confetti[i].ay-=4; confetti[i].y++; }
					if(confetti[i].y > SCREEN_HEIGHT) confetti[i].y = -rand()%50 - 50;
				}
			}
			if(intro_timer(2, 4)) {
				for(i=0 ; i<NB_CONFETTI ; i++) {
					confetti[i].ax += abs(confetti[i].dx);
					if(confetti[i].ax >= 4) { confetti[i].ax-=4; confetti[i].x+=sgn(confetti[i].dx); }
				}
			}
			if(intro_timer(3, 20)) {
				for(i=0 ; i<NB_CONFETTI ; i++) {
					if(abs(confetti[i].dx) == 4) confetti[i].ddx *= -1;
					confetti[i].dx += confetti[i].ddx;
				}
			}
			intro_background(1);
			CopySpriteMasked(congratulations->tile[0], congratulations->palette, (SCREEN_WIDTH-congratulations->width)/2, 17, congratulations->width, congratulations->height, MASK);
			CopySpriteMasked(game_complete->tile[0], game_complete->palette, (SCREEN_WIDTH-game_complete->width)/2, 56, game_complete->width, game_complete->height, MASK);
			CopySpriteMasked(hat_duck->tile[0], hat_duck->palette, (SCREEN_WIDTH-hat_duck->width)/2, 87, hat_duck->width, hat_duck->height, MASK);
			CopySpriteMasked(button->tile[0], button->palette, (SCREEN_WIDTH-button->width)/2, 176, button->width, button->height, MASK);
			text_print((LCD_WIDTH_PX-text_width("MAIN MENU"))/2, 176+9, "MAIN MENU", 1, MASK);
			for(i=0 ; i<NB_CONFETTI ; i++) Rectangle(confetti[i].x, confetti[i].y, 4, 4, confetti_color[confetti[i].color]);
			Bdisp_PutDisp_DD();
		}
	}

	tileset_free(congratulations);
	tileset_free(game_complete);
	tileset_free(hat_duck);
	tileset_free(button);
}

static int intro_timer(int id, int delay)
{
	static int time[4]={0};
	if(!time[id]) time[id] = time_getTicks();
	if(time_getTicks() - time[id] > delay) {
		time[id] = time_getTicks();
		return 1;
	}
	return 0;
}

static void intro_background(int end)
{
	int x, y;
	const short color_intro[] = {4129,6241,8386,12547,14660,18853,23014}, color_end[] = {11449,11515,11614,15807,22015,28223,32351};
	const short *color;
	short *vram = GetVRAMAddress() + 64;
	color = end ? color_end : color_intro;
	for(y=0 ; y<SCREEN_HEIGHT ; y++) {
		for(x=0 ; x<SCREEN_WIDTH ; x++)
			*(vram++) = color[y>>5];
		vram += 64;
	}
}
