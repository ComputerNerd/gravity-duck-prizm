#ifndef _OVERCLOCK
#define _OVERCLOCK

#define PLL_28x 0b011011 // 101.5 MHz (NOT SAFE)
#define PLL_26x 0b011001 // 94.3 MHz
#define PLL_24x 0b010111 // 87 MHz
#define PLL_20x 0b010011 // 72.5 MHz
#define PLL_18x 0b010001 // 65.25 MHz
#define PLL_16x 0b001111 // 58 MHz (NORMAL SPEED)
#define PLL_15x 0b001110 // 54.37 MHz
#define PLL_12x 0b001011 // 43.5 MHz
#define PLL_8x  0b000111 // 29 MHz
#define PLL_6x  0b000101 // 21.75 MHz
#define PLL_4x  0b000011 // 14.5 MHz
#define PLL_3x  0b000010 // 10.8 MHz
#define PLL_2x  0b000001 // 7.25 MHz
#define PLL_1x  0b000000 // 3.6 MHz

void overclock(int mult);

#endif //_OVERCLOCK
