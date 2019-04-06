#include "text.h"
#include "graphic_functions.h"

//Local variables
#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216

/* Modified by Nemh (09 april 2017) to enable full compatibility between fxcg20 and fxcg50
   VRAM adress was originaly hardcoded thus leading to some issues regardind text printing */
#define VRAM GetVRAMAddress()
//#define VRAM 0xA8000000

Font default_font = { default_data, 5, 5, 0 };
Font* used_font = &default_font;

//private functions
static void text_drawPoint(int x, int y, int size, int color, int alpha)
{
	int i, j;
	short* vram = VRAM;
	if(x+size>=LCD_WIDTH_PX || x<0 || y+size>=LCD_HEIGHT_PX || y<0) return;
	vram += y*LCD_WIDTH_PX + x;
	if(alpha == 32) {
		for(i=size ; i ; i--, vram+=LCD_WIDTH_PX-size) {
			for(j=size ; j ; j--) {
				*(vram++) = color;
			}
		}
	} else {
		for(i=size ; i ; i--, vram+=LCD_WIDTH_PX-size) {
			for(j=size ; j ; j--) {
				*vram = ((((color & 0xf81f) * alpha + (*vram & 0xf81f) * (32-alpha)) >> 5) & 0xf81f) |
							((((color & 0x07e0) * alpha + (*vram & 0x07e0) * (32-alpha)) >> 5) & 0x07e0);
				vram++;
			}
		}
	}
}

static int text_readPix(const char* data, int x, int y, int w, int h)
{
	int byte_width = ((w-1)>>3)+1;
	if(x<0 || x>=w || y<0 || y>=h) return 0;
	return 0 != (data[y*byte_width+(x>>3)] & (128>>(x&7)));
}

static void text_antialias(int x, int y, int size, int color, int alpha, int corner)
{
	int i, j, m=size>>1;
	switch(corner) {
		case 0: //top left
			for(i=0 ; i<size-m ; i++) {
				for(j=0 ; j<size-i-m-1 ; j++)
					text_drawPoint(x+j, y+i, 1, color, alpha);
				text_drawPoint(x+j, y+i, 1, color, alpha>>1);
			}
			break;
		case 1: //top right
			for(i=0 ; i<size-m ; i++) {
				text_drawPoint(x+i+m, y+i, 1, color, alpha>>1);
				for(j=i+m+1 ; j<size ; j++)
					text_drawPoint(x+j, y+i, 1, color, alpha);
			}
			break;
		case 2: //bottom left
			for(i=m ; i<size ; i++) {
				for(j=0 ; j<i-m ; j++)
					text_drawPoint(x+j, y+i, 1, color, alpha);
				text_drawPoint(x+j, y+i, 1, color, alpha>>1);
			}
			break;
		case 3: //bottom right
			for(i=m-1 ; i<size ; i++) {
				text_drawPoint(x+size-i+m-1, y+i, 1, color, alpha>>1);
				for(j=size-i+m ; j<size ; j++)
					text_drawPoint(x+j, y+i, 1, color, alpha);
			}
			break;
	}
}

//public functions
void text_setFont(Font* font)
{
	used_font = font;
}

Font* text_getActualFont()
{
	return used_font;
}

void text_printC(int x, int y, char c, int size, int color)
{
	int i, j, byte_width, alpha;
	const char* data;
	if(c<32 || size<1) return;
	byte_width = ((used_font->width-1)>>3)+1;
	data = used_font->data + byte_width * used_font->height * (c-32);
	alpha = 32 - ((color>>16) % 32);
	color &= 0xFFFF;
	for(i=0 ; i<used_font->height ; i++) {
		for(j=0 ; j<used_font->width ; j++) {
			if(data[i*byte_width+(j>>3)] & (128>>(j&7)))
				text_drawPoint(x+j*size, y+i*size, size, color, alpha);
			else if(used_font->flags & ANTIALIASING) { // Antialiasing
				if(text_readPix(data, j, i-1, used_font->width, used_font->height)) {
					if(text_readPix(data, j-1, i, used_font->width, used_font->height)) text_antialias(x+j*size, y+i*size, size, color, alpha, 0);
					if(text_readPix(data, j+1, i, used_font->width, used_font->height)) text_antialias(x+j*size, y+i*size, size, color, alpha, 1);
				}
				if(text_readPix(data, j, i+1, used_font->width, used_font->height)) {
					if(text_readPix(data, j-1, i, used_font->width, used_font->height)) text_antialias(x+j*size, y+i*size, size, color, alpha, 2);
					if(text_readPix(data, j+1, i, used_font->width, used_font->height)) text_antialias(x+j*size, y+i*size, size, color, alpha, 3);
				}
			}
		}
	}
}

void text_print(int x, int y, const char* c, int size, int color)
{
	int save_x = x;
	for( ; *c ; c++) {
		if(*c == '\n') {
			x = save_x;
			y += (used_font->height + used_font->height/2) * size;
		} else {
			text_printC(x, y, *c, size, color);
			x += size * text_widthC(*c);
		}
	}
}

int text_widthC(char c)
{
	if(used_font->flags & MONOSPACE || c==' ') return used_font->width+1;
	const char *data;
	unsigned char col=0;
	int i, j, byte_width, width;
	byte_width = ((used_font->width-1)>>3)+1;
	width = 8 * byte_width;
	data = used_font->data + byte_width * used_font->height * (c-32);
	for(j=byte_width-1 ; j>=0 && !col ; j--) {
		col = 0;
		for(i=0 ; i<used_font->height ; i++) {
			col |= data[i*byte_width + j];
		}
		if(col) {
			while(!(col&1)) {
				col>>=1;
				width--;
			}
		} else width -= 8;
	}
	return width + 1;
}

int text_width(const char* c)
{
	int width = 0;
	for( ; *c ; c++) width += text_widthC(*c);
	return width;
}
