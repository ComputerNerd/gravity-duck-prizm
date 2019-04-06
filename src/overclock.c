#include "overclock.h"

void overclock(int mult)
{
	asm("mov r4, r0\n"
	    "and #0x3F, r0\n"
	    "shll16 r0\n"
	    "shll8 r0\n"
	    "mov.l frqcr, r1\n"
	    "mov.l pll_mask, r3\n"
	    "mov.l @r1, r2\n"
	    "and r3, r2\n"
	    "or r0, r2\n"
	    "mov.l r2, @r1\n"
	    "mov.l frqcr_kick_bit, r0\n"
	    "mov.l @r1, r2\n"
	    "or r0, r2\n"
	    "rts\n"
	    "mov.l r2, @r1\n"
	    ".align 4\n"
	    "frqcr_kick_bit: .long 0x80000000\n"
	    "pll_mask: .long 0xC0FFFFFF\n"
	    "frqcr: .long 0xA4150000"
	);
}
