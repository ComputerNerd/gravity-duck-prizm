#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <fxcg/misc.h>
#include "graphic_functions.h"

#include "draw.h"
#include "key.h"
#include "level.h"
#include "menu.h"
#include "tileset.h"
#include "text.h"
#include "time.h"

#define BG_COLOR 0x869C
#define CIRCLE_COLOR 0xCF1C
#define NB_CIRCLE 9
#define NB_ROW 8
#define NB_COL 5
#define NB_DISPLAYED_LEVEL (NB_ROW*NB_COL)
#define ITEM_WIDTH 20
#define ITEM_HEIGHT 20
#define MENU_WIDTH (NB_ROW*(ITEM_WIDTH+1))
#define MENU_HEIGHT (NB_COL*(ITEM_HEIGHT+1))
#define MENU_X ((SCREEN_WIDTH - MENU_WIDTH)/2)
#define MENU_Y 89

static void menu_background(void);

/* public functions */
void menu_title()
{
	Tileset *title = tileset_load(TITLE);
	if(!title) return;
	while(key_down(K_EXE) || key_down(K_EXIT));
	while(!key_down(K_EXE) && !key_down(K_EXIT)) {
		FillScreen(BG_COLOR);
		menu_background();
		Shadow(title->tile[0], title->palette, (SCREEN_WIDTH-title->width)/2-5, (SCREEN_HEIGHT-title->height)/2+5, title->width, title->height, MASK);
		CopySpriteMasked(title->tile[0], title->palette, (SCREEN_WIDTH-title->width)/2, (SCREEN_HEIGHT-title->height)/2, title->width, title->height, MASK);
		Bdisp_PutDisp_DD();
	}
	tileset_free(title);
}

int menu_main(int cursor, int unlock)
{
	int i, x, y, time, ret=-1, first=1;
	char str[3];
	Tileset *select_level, *bg, *button;

	select_level = tileset_load(SELECT_LEVEL);
	bg = tileset_load(SELECT_LEVEL_BG);
	button = tileset_load(BUTTON);
	if(select_level && bg && button) {
		if(cursor > NB_LEVEL) cursor = NB_LEVEL;
		if(unlock > NB_LEVEL) unlock = NB_LEVEL;
		while(key_down(K_EXE) || key_down(K_EXIT));
		time = time_getTicks();
		while(ret<0) {
			if(time_getTicks() - time > 10) {
				if(key_down(K_EXIT)) ret = 0;
				if(key_down(K_EXE)) ret = cursor;
				if(key_down(K_LEFT) && cursor>1) cursor--;
				if(key_down(K_RIGHT) && cursor<unlock) cursor++;
				if(key_down(K_UP) && cursor>NB_ROW) cursor-=NB_ROW;
				if(key_down(K_DOWN) && cursor+NB_ROW<=unlock) cursor+=NB_ROW;
				if(cursor-first < NB_ROW  && first > 1) first -= NB_ROW;
				if(cursor-first >= NB_DISPLAYED_LEVEL-NB_ROW && first < NB_LEVEL-NB_DISPLAYED_LEVEL) first += NB_ROW;
				time = time_getTicks();
			}
			FillScreen(BG_COLOR);
			menu_background();
			Shadow(select_level->tile[0], select_level->palette, (SCREEN_WIDTH-select_level->width)/2-5, 13+5, select_level->width, select_level->height, MASK);
			CopySpriteMasked(select_level->tile[0], select_level->palette, (SCREEN_WIDTH-select_level->width)/2, 13, select_level->width, select_level->height, MASK);
			Shadow(bg->tile[0], bg->palette, (SCREEN_WIDTH-bg->width)/2-5, 77+5, bg->width, bg->height, MASK);
			CopySpriteMasked(bg->tile[0], bg->palette, (SCREEN_WIDTH-bg->width)/2, 77, bg->width, bg->height, MASK);
			for(i=0 ; i<NB_DISPLAYED_LEVEL && i+first <= NB_LEVEL ; i++) {
				x = MENU_X+(i%NB_ROW)*(ITEM_WIDTH+1);
				y = MENU_Y+(i/NB_ROW)*(ITEM_HEIGHT+1);
				CopySpriteMasked(button->tile[ i+first<=unlock? (i+first==cursor? 0 : 1) : 2 ], button->palette, x, y, button->width, button->height, MASK);
				if(i+first<=unlock) {
					itoa(i+first, str);
					text_print(x+5+(i+first<4)+(i+first<10)+(i+first<20)+((i+first)%10==1)+32, y+8, str, 1, MASK);
				}
			}
			Bdisp_PutDisp_DD();
		}
	} else {
		PrintXY(2, 2, "  LvlSel Fail", TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
		int key_unused;
		for(;;) {
			GetKey(&key_unused);
		}
	}
	tileset_free(select_level);
	tileset_free(bg);
	tileset_free(button);
	return ret;
}

/* private functions */
static void menu_background(void)
{
	static int time=0, x[NB_CIRCLE]={42, 72, 223, 265, 358, 396, 506, 587, 606}, y[NB_CIRCLE]={163, 31, 239, 109, 0, 185, 83, 230, 38}, size[NB_CIRCLE]={35, 55, 60, 51, 34, 50, 65, 53, 27};
	int i;
	if(time_getTicks() - time > 5) {
		for(i=0 ; i<NB_CIRCLE ; i++) {
			x[i]++;
			if(x[i]-160 >= SCREEN_WIDTH)
				x[i] -= 640;
		}
		time = time_getTicks();
	}
	for(i=0 ; i<NB_CIRCLE ; i++)
		FilledCircle(x[i], y[i], size[i] , CIRCLE_COLOR);
}
