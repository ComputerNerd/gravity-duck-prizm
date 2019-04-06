#include <fxcg/display.h>
#include "draw.h"
#include "graphic_functions.h"

void FillVRAM(short color)
{
	int i, color32bits, *VRAM = GetVRAMAddress();
	color32bits = (color<<16) | color;
	for(i=LCD_WIDTH_PX*LCD_HEIGHT_PX/2 ; i ; i--)
		*(VRAM++) = color32bits;
}

void Fade(void)
{
	int i;
	short* VRAM = GetVRAMAddress();
	for(i=LCD_WIDTH_PX*LCD_HEIGHT_PX ; i ; i--)
	{
		*VRAM = (((*VRAM & 0xF81F) >> 1) & 0xF81F) | (((*VRAM & 0x07E0) >> 1) & 0x07E0);
		VRAM++;
	}
}

// Custom function for 320*216 screen
void FillScreen(unsigned short color)
{
	int i, j, color32bits, *VRAM = GetVRAMAddress();
	color32bits = (color<<16) | color;
	VRAM += 16;
	for(i=LCD_HEIGHT_PX ; i ; i--, VRAM+=32) {
		for(j=LCD_WIDTH_PX/2 - 32 ; j ; j--)
			*(VRAM++) = color32bits;
	}
}

// Custom function for 320*216 screen
void HorizontalLine(int y, int x1, int x2, short color)
{
	int i;
	short *VRAM = GetVRAMAddress();
    if(y<0 || y>SCREEN_HEIGHT || (x1<0 && x2<0) || (x1>SCREEN_WIDTH && x2>SCREEN_WIDTH)) return;
    if(x1 > x2) { i = x1; x1 = x2; x2 = i; }
    if(x1 < 0) x1 = 0;
    if(x2 >= SCREEN_WIDTH) x2 = SCREEN_WIDTH-1;
	VRAM += y*LCD_WIDTH_PX + x1 + 32;
	for(i=x2-x1+1 ; i ; i--)
		(*VRAM++) = color;
}

void Rectangle(int x, int y, int w, int h, short color)
{
	for( ; h ; h--)
		HorizontalLine(y+h-1, x, x+w-1, color);
}

void FilledCircle(int x, int y, int radius, short color)
{
	int plot_x, plot_y, d;

	if(radius < 0) return;
	plot_x = 0;
	plot_y = radius;
	d = 1 - radius;

	HorizontalLine(y, x-plot_y, x+plot_y, color);
	while(plot_y > plot_x)
	{
		if(d < 0)
			d += 2*plot_x+3;
		else {
			d += 2*(plot_x-plot_y)+5;
			plot_y--;
			HorizontalLine(y+plot_y+1, x-plot_x, x+plot_x, color);
			HorizontalLine(y-plot_y-1, x-plot_x, x+plot_x, color);
		}
		plot_x++;
		if(plot_y >= plot_x) {
			HorizontalLine(y+plot_x, x-plot_y, x+plot_y, color);
			HorizontalLine(y-plot_x, x-plot_y, x+plot_y, color);
		}
	}
}

// Custom function for 320*216 screen
void CopySpriteMasked(const char* bitmap, const short* palette, int x, int y, int width, int height, short mask)
{
	short* VRAM = GetVRAMAddress();

	int y_index;
	int x_index;
	short * base = y * LCD_WIDTH_PX + x + VRAM+32;
	for (y_index = height; y_index > 0; --y_index, base += LCD_WIDTH_PX - width) {
		for (x_index = width; x_index > 0; --x_index, ++base, ++bitmap) {
			if (palette[(int)*bitmap] != mask) *base = palette[(int)*bitmap];
		}
	}
}

// Custom function for 320*216 screen
void CopySpriteClippingMasked(const char* bitmap, const short* palette, int x, int y, int width, int height, short mask)
{
	short* VRAM = GetVRAMAddress();
	int x_inc = width;
	if (y < 0) {
		bitmap -= y * width;
		height += y;
		y = 0;
	}
	if (height > LCD_HEIGHT_PX - y) height = LCD_HEIGHT_PX - y;

	if (x < 0) {
		bitmap -= x;
		width += x;
		x = 0;
	}
	if (width > SCREEN_WIDTH - x) width = SCREEN_WIDTH - x;

	x_inc -= width;

	int y_index;
	int x_index;
	short * base = y * LCD_WIDTH_PX + x + VRAM+32;
	for (y_index = height; y_index > 0; --y_index, base += LCD_WIDTH_PX - width, bitmap += x_inc) {
		for (x_index = width; x_index > 0; --x_index, ++base, ++bitmap) {
			if (palette[(int)*bitmap] != mask) *base = palette[(int)*bitmap];
		}
	}
}

// Custom function for 320*216 screen
void CopySpriteClippingMaskedAlpha(const char* bitmap, const short* palette, int x, int y, int width, int height, short mask, char alpha)
{
	short* VRAM = GetVRAMAddress();
	int x_inc = width;
	if (y < 0) {
		bitmap -= y * width;
		height += y;
		y = 0;
	}
	if (height > LCD_HEIGHT_PX - y) height = LCD_HEIGHT_PX - y;

	if (x < 0) {
		bitmap -= x;
		width += x;
		x = 0;
	}
	if (width > SCREEN_WIDTH - x) width = SCREEN_WIDTH - x;

	x_inc -= width;
	alpha %= 32;

	int y_index;
	int x_index;
	short * base = y * LCD_WIDTH_PX + x + VRAM+32;
	for (y_index = height; y_index > 0; --y_index, base += LCD_WIDTH_PX - width, bitmap += x_inc) {
		for (x_index = width; x_index > 0; --x_index, ++base, ++bitmap) {
			if (palette[(int)*bitmap] != mask) *base = ((((palette[(int)*bitmap] & 0xF81F) * alpha + (*base & 0xF81F) * (32-alpha)) >> 5) & 0xF81F) |
			                                      ((((palette[(int)*bitmap] & 0x07E0) * alpha + (*base & 0x07E0) * (32-alpha)) >> 5) & 0x07E0);
		}
	}
}

// Custom function for 320*216 screen
void Shadow(const char* bitmap, const short* palette, int x, int y, int width, int height, short mask)
{
	short* VRAM = GetVRAMAddress();

	int y_index;
	int x_index;
	short * base = y * LCD_WIDTH_PX + x + VRAM+32;
	for (y_index = height; y_index > 0; --y_index, base += LCD_WIDTH_PX - width) {
		for (x_index = width; x_index > 0; --x_index, ++base, ++bitmap) {
			if (palette[(int)*bitmap] != mask) *base = (((*base & 0xF81F)>>1) & 0xF81F) | (((*base & 0x07E0)>>1) & 0x07E0);
		}
	}
}
