#ifndef _TEXT
#define _TEXT

typedef struct {
	const char* data;
	int width, height, flags;
} Font;


// Functions
void text_printC(int x, int y, char c, int size, int color);
void text_print(int x, int y, const char* c, int size, int color);
int text_widthC(char c);
int text_width(const char* c);

/*
color on 32bits : 16 bits for alpha transparency + 16 bits for color.
alpha=0 mean opaque, alpha=32 mean totaly transparent.
example : 0x0010FFFF mean "white, 50% opacity"
*/

#endif //_TEXT
