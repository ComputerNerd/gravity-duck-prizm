#include "text.h"
#include "graphic_functions.h"

//Local variables
#define LCD_WIDTH_PX 384
#define LCD_HEIGHT_PX 216

/* Modified by Nemh (09 april 2017) to enable full compatibility between fxcg20 and fxcg50
   VRAM adress was originaly hardcoded thus leading to some issues regardind text printing */
#define VRAM GetVRAMAddress()
//#define VRAM 0xA8000000

// Flags :
#define MONOSPACE    0x00000001
#define ANTIALIASING 0x00000002

static const char default_data[] = {
	0,0,0,0,0,           // ' '
	128,128,128,0,128,   // '!'
	160,160,0,0,0,       // '"'
	80,248,80,248,80,    // '#'
	64,224,64,224,64,    // '$'
	200,208,32,88,152,   // '%'
	0,96,64,144,96,      // '&'
	128,128,0,0,0,       // '''
	64,128,128,128,64,   // '('
	128,64,64,64,128,    // ')'
	64,224,64,160,0,     // '*'
	32,32,248,32,32,     // '+'
	0,0,0,128,128,       // ','
	0,0,248,0,0,         // '-'
	0,0,0,0,128,       // '.'
	8,16,32,64,128,      // '/'
	248,136,136,136,248, // '0'
	192,64,64,64,64,     // '1'
	240,16,240,128,240,  // '2'
	240,16,112,16,240,   // '3'
	128,136,136,248,8,   // '4'
	248,128,248,8,248,   // '5'
	248,128,248,136,248, // '6'
	248,8,8,8,8,         // '7'
	248,136,248,136,248, // '8'
	248,136,248,8,248,   // '9'
	0,0,128,0,128,       // ':'
	0,128,0,128,128,     // ';'
	32,64,128,64,32,     // '<'
	0,248,0,248,0,       // '='
	128,64,32,64,128,    // '>'
	224,16,96,0,64,      // '?'
	248,136,184,168,184, // '@'
	248,136,248,136,136, // 'A'
	240,136,248,136,240, // 'B'
	248,128,128,128,248, // 'C'
	240,136,136,136,240, // 'D'
	248,128,240,128,248, // 'E'
	248,128,240,128,128, // 'F'
	120,128,184,136,120, // 'G'
	136,136,248,136,136, // 'H'
	128,128,128,128,128, // 'I'
	16,16,16,144,96,     // 'J'
	136,144,224,144,136, // 'K'
	128,128,128,128,248, // 'L'
	136,216,168,136,136, // 'M'
	248,136,136,136,136, // 'N'
	248,136,136,136,248, // 'O'
	248,136,248,128,128, // 'P'
	112,136,136,152,120, // 'Q'
	248,136,248,144,136, // 'R'
	248,128,248,8,248,   // 'S'
	248,32,32,32,32,     // 'T'
	136,136,136,136,248, // 'U'
	136,136,80,80,32,    // 'V'
	168,168,168,168,248, // 'W'
	136,80,32,80,136,    // 'X'
	136,80,32,32,32,     // 'Y'
	248,8,112,128,248,   // 'Z'
	192,128,128,128,192, // '['
	128,64,32,16,8,      // '\'
	192,64,64,64,192,    // ']'
	64,160,0,0,0,        // '^'
	0,0,0,0,248,         // '_'
	128,64,0,0,0,        // '`'
	0,112,144,144,112,   // 'a'
	128,224,144,144,224, // 'b'
	0,96,128,128,96,     // 'c'
	16,112,144,144,112,  // 'd'
	96,144,240,128,112,  // 'e'
	48,64,224,64,64,     // 'f'
	112,144,112,16,224,  // 'g'
	128,128,224,144,144, // 'h'
	128,0,128,128,128,   // 'i'
	64,0,64,64,128,      // 'j'
	128,128,160,192,160, // 'k'
	128,128,128,128,128, // 'l'
	0,0,240,168,168,     // 'm'
	0,0,224,144,144,     // 'n'
	0,96,144,144,96,     // 'o'
	0,240,144,240,128,   // 'p'
	0,240,144,240,16,    // 'q'
	0,176,192,128,128,   // 'r'
	0,112,64,32,224,     // 's'
	0,128,224,128,96,    // 't'
	0,0,144,144,96,      // 'u'
	0,0,136,80,32,       // 'v'
	0,0,168,168,80,      // 'w'
	0,0,144,96,144,      // 'x'
	0,160,224,32,192,    // 'y'
	0,240,32,64,240,     // 'z'
	96,64,192,64,96,     // '{'
	128,128,128,128,128, // '|'
	192,64,96,64,192,    // '}'
	0,0,80,160,0         // '~'
};

static const Font default_font = { default_data, 5, 5, 0 };
static const Font* used_font = &default_font;

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
