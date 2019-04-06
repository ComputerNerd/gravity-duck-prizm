#include <fxcg/display.h>
#include "graphic_functions.h"

#include "draw.h"
#include "engine.h"
#include "key.h"
#include "level.h"
#include "object.h"
#include "rand.h"
#include "tileset.h"
#include "text.h"
#include "time.h"

#define SPEED_FALLING 3
#define SPEED_BULLET 4
#define sgn(x) (x<0?-1:x>0?1:0)
typedef enum {QUIT, RETRY, NEXT} Return_code;

static Return_code engine_loop(Level* level);
static int engine_pause();
static void engine_tips(Level* level);
static void engine_movements(Level* level);
static void engine_move_objects(Level* level);
static void engine_anim(Level* level);
static int engine_collisions(Level* level);
static int engine_hitMap(Level* level, Object* object, int dx, int dy);

char display_fps=0;

char *itoa(int, char *);

/* public functions */
int engine_main(int id_level)
{
	int run=1;
	Level* level;
	if(draw_init()) {
		int key;
		Bdisp_AllClr_VRAM();
		text_print(10, 50, "ERROR:\nNOT ENOUGH MEMORY\nTO LOAD ALL BITMAPS.\nPRESS [MENU] TO QUIT.", 3, 0);
		while(1) GetKey(&key);
	}
	while(run) {
		if((level = level_load(id_level))) {
			switch(engine_loop(level)) {
				case QUIT: run = 0; break;
				case NEXT: id_level++; break;
			}
			level_free(level);
		} else run = 0;
	}
	draw_quit();
	return id_level;
}

/* private functions */
static Return_code engine_loop(Level* level)
{
	int disp_fps=0, fps=0, time=0, optn=0;
	char str_lvl[7]="LVL ", str_fps[8]="FPS ";
	Camera camera;
	Object* duck = object_get(level->object, DUCK);
	camera.x = -(SCREEN_WIDTH-level->width*TILE_WIDTH)/2;
	camera.y = -(SCREEN_HEIGHT-level->height*TILE_HEIGHT)/2;

	str_lvl[4] = '0'+level->id/10;
	str_lvl[5] = '0'+level->id%10;
	while(1) {
		if(key_down(K_EXIT)) {
			if(engine_pause()) return QUIT;
		}
		if(key_down(K_OPTN)) { if(!optn) display_fps=!display_fps; optn=1; }
		else optn=0;

		engine_move_objects(level);
		engine_anim(level);
		engine_movements(level);

		if(engine_collisions(level)) duck->type = DUCK_BLAST;
		if(duck->var == 32) return RETRY;

		if(time_getTicks() - time > 128) {
			disp_fps = fps;
			itoa(disp_fps, str_fps+4);
			fps = 1;
			time = time_getTicks();
		} else fps++;

		draw_background();
		draw_tileMap(level, camera);
		draw_objects(level, camera);
		text_print(33, 2, str_lvl, 1, MASK);
		if(display_fps) text_print(33, 9, str_fps, 1, MASK);
		engine_tips(level);
		Bdisp_PutDisp_DD();

		if(object_get(level->object, EGG)->position == 19) return NEXT;
	}
}

static int engine_pause()
{
	int cursor=0;
	Tileset *pause, *pause_button;
	draw_quit(); // free memory to load pause menu
	pause = tileset_load(PAUSE);
	pause_button = tileset_load(PAUSE_BUTTON);
	if(pause && pause_button) {
		Fade();
		while(!key_down(K_EXE)) {
			CopySpriteMasked(pause->tile[0], pause->palette, (SCREEN_WIDTH-pause->width)/2, (SCREEN_HEIGHT-pause->height)/2, pause->width, pause->height, MASK);
			CopySpriteMasked(pause_button->tile[0], pause_button->palette, (SCREEN_WIDTH-pause->width)/2+7, (SCREEN_HEIGHT-pause->height)/2+34+25*cursor, pause_button->width, pause_button->height, MASK);
			text_print((SCREEN_WIDTH-pause->width)/2+19+32, (SCREEN_HEIGHT-pause->height)/2+42, "CONTINUE", 1, MASK);
			text_print((SCREEN_WIDTH-pause->width)/2+15+32, (SCREEN_HEIGHT-pause->height)/2+67, "MAIN MENU", 1, MASK);
			Bdisp_PutDisp_DD();
			if(key_down(K_UP) && cursor) cursor--;
			if(key_down(K_DOWN) && !cursor) cursor++;
		}
	}
	tileset_free(pause);
	tileset_free(pause_button);
	draw_init();
	return cursor;
}

static void engine_tips(Level* level)
{
	if(!level->tips) return;
	int i, n=1;
	char *str[3];
	draw_tipsPanel();
	str[0] = level->tips;
	for(i=0 ; level->tips[i] ; i++) {
		if(level->tips[i] == '\n') {
			level->tips[i] = '\0';
			if(n<3) {
				str[n] = &level->tips[i+1];
				n++;
			}
		}
	}
	for(i=0 ; i<n ; i++)
		text_print((LCD_WIDTH_PX-text_width(str[i]))/2, 23-(5*(2*n-1))/2 + 10*i, str[i], 1, MASK);
	for(i=1 ; i<n ; i++) *(str[i]-1) = '\n';
}

static void engine_movements(Level* level)
{
	static int time=0;
	static int shift_pressed=0;
	int i;
	Object* duck;

	if(time_getTicks() - time > 1) {
		time = time_getTicks();
		duck = object_get(level->object, DUCK);
		duck->dx -= sgn(duck->dx);
		duck->dy -= sgn(duck->dy);
		if(level->gravity == UP || level->gravity == DOWN) {
			if(key_down(K_LEFT)) { duck->look=LEFT; if(!engine_hitMap(level, duck, -2, 0)) duck->dx=-2; }
			else if(key_down(K_RIGHT)) { duck->look=RIGHT; if(!engine_hitMap(level, duck, 2, 0)) duck->dx=2; }
			else duck->position = 0;
		} else {
			if(key_down(K_UP)) { duck->look=UP; if(!engine_hitMap(level, duck, 0, -2)) duck->dy=-2; }
			else if(key_down(K_DOWN)) { duck->look=DOWN; if(!engine_hitMap(level, duck, 0, 2)) duck->dy=2; }
			else duck->position = 0;
		}
		if(engine_hitMap(level, duck, (level->gravity==LEFT?-1:level->gravity==RIGHT?1:0), (level->gravity==UP?-1:level->gravity==DOWN?1:0))) {
			if(key_down(K_SHIFT)) {
				if(!shift_pressed) {
					duck->head = (duck->head+2)%4;
					level->gravity = (level->gravity+2)%4;
					shift_pressed = 1;
				}
			} else shift_pressed = 0;
		} else duck->position = 0;
		if(object_get(level->object, EGG)->position < 16) {
			if(!engine_hitMap(level, duck, duck->dx, duck->dy)) {
				duck->x += duck->dx;
				duck->y += duck->dy;
			}
		} else duck->position = 0;
		for(i=SPEED_FALLING ; i>0 ; i--) {
			switch(level->gravity) {
				case UP: if(!engine_hitMap(level, duck, 0, -i)) duck->y-=i, i=0; break;
				case DOWN: if(!engine_hitMap(level, duck, 0, i)) duck->y+=i, i=0; break;
				case LEFT: if(!engine_hitMap(level, duck, -i, 0)) duck->x-=i, i=0; break;
				case RIGHT: if(!engine_hitMap(level, duck, i, 0)) duck->x+=i, i=0; break;
			}
		}
	}
}

static void engine_move_objects(Level* level)
{
	static int time=0;
	int i, j, n, min_distance, distance;
	char pestle_hitbox[] = {31, 27, 22, 18, 14, 10};
	char homing_bullet[28][2] = {{5, 0}, {5, 1}, {5, 2}, {4, 3}, {3, 4}, {2, 5}, {1, 5}, {0, 5}, {-1, 5}, {-2, 5}, {-3, 4}, {-4, 3}, {-5, 2}, {-5, 1}, {-5, 0}, {-5, -1}, {-5, -2}, {-4, -3}, {-3, -4}, {-2, -5}, {-1, -5}, {0, -5}, {1, -5}, {2, -5}, {3, -4}, {4, -3}, {5, -2}, {5, -1}};
	Object* object, *new, *duck = object_get(level->object, DUCK);
	if(time_getTicks() - time > 2) { // 4
		time = time_getTicks();
		engine_anim(level);
		for(i=0 ; level->object[i] ; i++) {
			object = level->object[i];
			switch(object->type) {
				case CUBE:
					if(engine_hitMap(level, object, object->dx, object->dy)) object->dx*=-1, object->dy*=-1;
					object->x+=object->dx;
					object->y+=object->dy;
					break;
				case CATERPILLAR:
					switch(object->head) {
						case UP: if(engine_hitMap(level, object, object->dx, 0) || !engine_hitMap(level, object, object->dx*TILE_WIDTH, TILE_HEIGHT)) object->dx*=-1, object->look = (object->look+2)%4; break;
						case DOWN: if(engine_hitMap(level, object, object->dx, 0) || !engine_hitMap(level, object, object->dx*TILE_WIDTH, -TILE_HEIGHT)) object->dx*=-1, object->look = (object->look+2)%4; break;
						case LEFT: if(engine_hitMap(level, object, 0, object->dy) || !engine_hitMap(level, object, TILE_WIDTH, object->dy*TILE_HEIGHT)) object->dy*=-1, object->look = (object->look+2)%4; break;
						case RIGHT: if(engine_hitMap(level, object, 0, object->dy) || !engine_hitMap(level, object, -TILE_WIDTH, object->dy*TILE_HEIGHT)) object->dy*=-1, object->look = (object->look+2)%4; break;
					}
					object->x+=object->dx;
					object->y+=object->dy;
					break;
				case SHOOTING_PLANT:
					if(object->var == TIME_PLANT-1) {
						object->var = 0;
						if(object->look == LEFT) {
							new = object_new(BULLET, object->x-1+SPEED_BULLET, object->y+5, UP, UP);
							new->dx = -SPEED_BULLET;
						} else {
							new = object_new(BULLET, object->x+13-SPEED_BULLET, object->y+5, UP, UP);
							new->dx = SPEED_BULLET;
						}
						level->object = object_add(level->object, new);
					}
					break;
				case BEE:
					if(duck->x < object->x) object->look = LEFT;
					else object->look = RIGHT;
					if(object->var == TIME_BEE-1) {
						object->var = 0;
						new = object_new(HOMING_BULLET, object->x+7, object->y+7, UP, UP);
						n = 0;
						min_distance = (duck->x-object->x-homing_bullet[0][0])*(duck->x-object->x-homing_bullet[0][0]) + (duck->y-object->y-homing_bullet[0][1])*(duck->y-object->y-homing_bullet[0][1]);
						for(j=0 ; j<28 ; j++) {
							distance = (duck->x-object->x-homing_bullet[j][0])*(duck->x-object->x-homing_bullet[j][0]) + (duck->y-object->y-homing_bullet[j][1])*(duck->y-object->y-homing_bullet[j][1]);
							if(distance < min_distance) {
								min_distance = distance;
								n = j;
							}
						}
						new->position = n;
						new->dx = homing_bullet[n][0];
						new->dy = homing_bullet[n][1];
						level->object = object_add(level->object, new);
					}
					break;
				case BULLET:
					object->x+=object->dx;
					if(object->x<0 || object->x+object->hitbox.x2>=level->width*TILE_WIDTH) {
						object->type = BULLET_BLAST;
						object->var = 0;
					} else if(engine_hitMap(level, object, 0, 0)) {
						object->type = BULLET_BLAST;
						object->var = 0;
					}
					break;
				case HOMING_BULLET:
					if(object->var == 6) {
						min_distance = (duck->x-object->x-homing_bullet[(int)object->position][0])*(duck->x-object->x-homing_bullet[(int)object->position][0]) + (duck->y-object->y-homing_bullet[(int)object->position][1])*(duck->y-object->y-homing_bullet[(int)object->position][1]);
						n = (object->position + 1)%28;
						distance = (duck->x-object->x-homing_bullet[n][0])*(duck->x-object->x-homing_bullet[n][0]) + (duck->y-object->y-homing_bullet[n][1])*(duck->y-object->y-homing_bullet[n][1]);
						if(distance < min_distance) object->position = n;
						n = (object->position + 27)%28;
						distance = (duck->x-object->x-homing_bullet[n][0])*(duck->x-object->x-homing_bullet[n][0]) + (duck->y-object->y-homing_bullet[n][1])*(duck->y-object->y-homing_bullet[n][1]);
						if(distance < min_distance) object->position = n;
						object->dx = homing_bullet[(int)object->position][0];
						object->dy = homing_bullet[(int)object->position][1];
						object->var = 0;
					} else object->var++;
					object->x+=object->dx;
					object->y+=object->dy;
					if(object->x<0 || object->y<0 || object->x+object->hitbox.x2>=level->width*TILE_WIDTH || object->y+object->hitbox.y2>=level->height*TILE_HEIGHT) {
						object->type = BULLET_BLAST;
						object->var = 0;
					} else if(engine_hitMap(level, object, 0, 0)) {
						object->type = BULLET_BLAST;
						object->var = 0;
					}
					break;
				case PESTLE:
					switch(object->head) {
						case UP: object->hitbox.y1 = object->hitbox.y2-pestle_hitbox[(int)object->position]; break;
						case DOWN: object->hitbox.y2 = object->hitbox.y1+pestle_hitbox[(int)object->position]; break;
						case LEFT: object->hitbox.x1 = object->hitbox.x2-pestle_hitbox[(int)object->position]; break;
						case RIGHT: object->hitbox.x2 = object->hitbox.x1+pestle_hitbox[(int)object->position]; break;
					}
					break;
				case DUCK_BLAST: if(object->var > 32) { level->object = object_del(level->object, object); i--; } break;
				case BULLET_BLAST: if(object->var > 32) { level->object = object_del(level->object, object); i--; } break;
			}
		}
	}
}

static void engine_anim(Level* level)
{
	static int time=0;
	int i;
	Object* object;
	if(time_getTicks() - time > 8) {
		time = time_getTicks();
		for(i=0 ; level->object[i] ; i++) {
			object = level->object[i];
			switch(object->type) {
				case DUCK: object->position++; if(object->position > 8) object->position = 1; break;
				case EGG: object->var = !object->var; if(object->position<15 || object->var) object->position++; if(object->position==15) object->position=0; break;
				case GRAVITY_SWITCH: object->position = (object->position-object->var+8)%8; break;
				case CATERPILLAR: object->position++; if(object->position > 5) object->position = 0; break;
				case SHOOTING_PLANT: if(object->var<6 && !(object->var&1)) object->position = (object->position+1)%3; object->var = (object->var+1)%TIME_PLANT; break;
				case BEE: object->position = (object->position+1)%4; object->var = (object->var+1)%TIME_BEE; break;
				case PESTLE: object->var = (object->var+1)%TIME_PESTLE; if(object->var < 6) object->position = (object->position+1)%6; break;
				case DUCK_BLAST: object->var+=4; break;
				case BULLET_BLAST: object->var+=4; break;
			}
		}
	}
}

static int engine_collisions(Level* level)
{
	int ret = 0;
	char item[4];
	int i, tmp, hit_gravity_switch=0;
	static int change_gravity=0;
	Object *object, *duck = object_get(level->object, DUCK);
	for(i=0 ; level->object[i] ; i++) {
		object = level->object[i];
		if(object_collision(duck, object)) {
			switch(object->type) {
				case GRAVITY_SWITCH:
					if(!change_gravity) {
						level->gravity = (level->gravity+object->var+4)%4;
						duck->head = (duck->head+object->var+4)%4;
						duck->look = (duck->look+object->var+4)%4;
						tmp=duck->hitbox.x1; duck->hitbox.x1=duck->hitbox.y1; duck->hitbox.y1=tmp; //rotate hitbox
						tmp=duck->hitbox.x2; duck->hitbox.x2=duck->hitbox.y2; duck->hitbox.y2=tmp; //rotate hitbox
						duck->x = object->x;
						duck->y = object->y;
						duck->y = object->y;
					}
					hit_gravity_switch=1;
					break;
				case EGG:
					if(duck->head == object->head && object->position<15)
						object->position = 16;
					break;
				case CUBE:
				case CATERPILLAR:
				case BULLET:
				case HOMING_BULLET:
				case PESTLE:
					ret = RETRY;
					break;
			}
		}
	}
	change_gravity = hit_gravity_switch ? 1 : 0;

	/* if duck touch the map border */
	if(duck->x < 4 || duck->y < 4 || duck->x > (level->width-1)*TILE_WIDTH-4 || duck->y > (level->height-1)*TILE_HEIGHT-4)
		return RETRY;

	/* collisions with spikes */
	item[0] = level->map[((duck->y+duck->hitbox.y1)/TILE_HEIGHT)*level->width + (duck->x+duck->hitbox.x1)/TILE_WIDTH];
	item[1] = level->map[((duck->y+duck->hitbox.y2)/TILE_HEIGHT)*level->width + (duck->x+duck->hitbox.x1)/TILE_WIDTH];
	item[2] = level->map[((duck->y+duck->hitbox.y1)/TILE_HEIGHT)*level->width + (duck->x+duck->hitbox.x2)/TILE_WIDTH];
	item[3] = level->map[((duck->y+duck->hitbox.y2)/TILE_HEIGHT)*level->width + (duck->x+duck->hitbox.x2)/TILE_WIDTH];
	for(i=0 ; i<4 ; i++) {
		switch(item[i]) {
			case SPIKE_UP: if((duck->y+duck->hitbox.y2)%TILE_HEIGHT >= TILE_HEIGHT/2) return RETRY; break;
			case SPIKE_DOWN: if((duck->y+duck->hitbox.y1)%TILE_HEIGHT < TILE_HEIGHT/2) return RETRY; break;
			case SPIKE_LEFT: if((duck->x+duck->hitbox.x2)%TILE_WIDTH >= TILE_WIDTH/2) return RETRY; break;
			case SPIKE_RIGHT: if((duck->x+duck->hitbox.x1)%TILE_WIDTH < TILE_WIDTH/2) return RETRY; break;
		}
	}
	return ret;
}

static int engine_hitMap(Level* level, Object* object, int dx, int dy)
{
	char item;
	item = level->map[((object->y+object->hitbox.y1+dy)/TILE_HEIGHT)*level->width + (object->x+object->hitbox.x1+dx)/TILE_WIDTH];
	if(item != EMPTY && item != SPIKE_UP && item != SPIKE_DOWN && item != SPIKE_LEFT && item != SPIKE_RIGHT) return 1;
	item = level->map[((object->y+object->hitbox.y2+dy)/TILE_HEIGHT)*level->width + (object->x+object->hitbox.x1+dx)/TILE_WIDTH];
	if(item != EMPTY && item != SPIKE_UP && item != SPIKE_DOWN && item != SPIKE_LEFT && item != SPIKE_RIGHT) return 1;
	item = level->map[((object->y+object->hitbox.y1+dy)/TILE_HEIGHT)*level->width + (object->x+object->hitbox.x2+dx)/TILE_WIDTH];
	if(item != EMPTY && item != SPIKE_UP && item != SPIKE_DOWN && item != SPIKE_LEFT && item != SPIKE_RIGHT) return 1;
	item = level->map[((object->y+object->hitbox.y2+dy)/TILE_HEIGHT)*level->width + (object->x+object->hitbox.x2+dx)/TILE_WIDTH];
	if(item != EMPTY && item != SPIKE_UP && item != SPIKE_DOWN && item != SPIKE_LEFT && item != SPIKE_RIGHT) return 1;
	return 0;
}
