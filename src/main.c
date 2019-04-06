#include "graphic_functions.h"

#include "engine.h"
#include "intro.h"
#include "key.h"
#include "level.h"
#include "menu.h"
#include "overclock.h"
#include "rand.h"
#include "save.h"
#include "time.h"


int main(void)
{
	int id_level, unlock, freq;

    if(GetVRAMAddress() == (void *)0xA8000000) {
    	srand(time_getTicks());
    	freq = (*((volatile unsigned int*)0xA4150000) & 0x3F000000) >> 24;
    	if(freq < PLL_26x) overclock(PLL_26x);
    }

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

    if(GetVRAMAddress() == (void *)0xA8000000) {
    	overclock(freq);
    }
	return 0;
}
