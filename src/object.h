#ifndef _OBJECT
#define _OBJECT

#define TIME_PLANT 25
#define TIME_BEE 50
#define TIME_PESTLE 11

typedef enum { DUCK, EGG, GRAVITY_SWITCH, CUBE, CATERPILLAR, SHOOTING_PLANT, BEE, BULLET, HOMING_BULLET, PESTLE, DUCK_BLAST, BULLET_BLAST } Object_type;

typedef enum {UP, LEFT, DOWN, RIGHT} Direction;

typedef struct {
	short x1, y1, x2, y2;
} Hitbox;

typedef struct {
	Object_type type;
	short x, y;
	char dx, dy, position, var;
	Hitbox hitbox;
	Direction head, look;
} Object;

int object_count(Object** list);
Object* object_get(Object** list, Object_type type);
Object** object_add(Object** list, Object* object);
Object** object_del(Object** list, Object* object);
Object* object_new(Object_type type, int x, int y, Direction head, Direction Look);
int object_collision(Object* obj1, Object* obj2);

#endif //_OBJECT
