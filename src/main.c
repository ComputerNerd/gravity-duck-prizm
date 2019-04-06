#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include "graphic_functions.h"

#include "engine.h"
#include "intro.h"
#include "key.h"
#include "level.h"
#include "menu.h"
#include "rand.h"
#include "save.h"
#include "time.h"


int main(void)
{
	Bdisp_EnableColor(1); // Otherwise GetKey puts the program into 3-bit mode.
	int id_level, unlock;

	FillVRAM(0x0000);

	menu_title();
	id_level = unlock = save_read();
	while((id_level = menu_main(id_level, unlock))) {
		if(id_level == 1) intro();
		id_level = engine_main(id_level);
		if(id_level > unlock) {
			unlock = id_level;
			save_write(unlock);
		}
		if(id_level > NB_LEVEL) {
			ending();
			menu_title();
		}
	}

	PrintXY(2, 2, "  Thank you", TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
	PrintXY(2, 3, "  for playing", TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
	PrintXY(2, 4, "  Gravity Duck!", TEXT_MODE_NORMAL, TEXT_COLOR_BLACK);
	for(;;) {
		int key;
		GetKey(&key);
	}
}
