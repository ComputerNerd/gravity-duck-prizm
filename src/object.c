#include <stdlib.h>

#include "object.h"

static Hitbox object_newHitbox(short x1, short y1, short x2, short y2);

/* public functions */
int object_count(Object** list)
{
	int i=0;
	while(list[i]) i++;
	return i;
}

Object* object_get(Object** list, Object_type type)
{
	int i=0;
	while(list[i] && list[i]->type!=type) i++;
	return list[i];
}

Object** object_add(Object** list, Object* object)
{
	int size;
	Object** tmp;
	size = object_count(list);
	if((tmp = realloc(list, (size+2)*sizeof(Object*)))) {
		list = tmp;
		list[size] = object;
		list[size+1] = 0;
	}
	return list;
}

Object** object_del(Object** list, Object* object)
{
	int i, size;
	size = object_count(list);
	for(i=0 ; list[i] && list[i]!=object ; i++); //find object
	if(list[i]) {
		free(object);
		for( ; i<size ; i++) list[i] = list[i+1]; //delete object
		list = realloc(list, size*sizeof(Object*)); //resize list
	}
	return list;
}

Object* object_new(Object_type type, int x, int y, Direction head, Direction look)
{
	Object* object;
	object = malloc(sizeof(Object));
	if(object) {
		object->type = type;
		object->x = x;
		object->y = y;
		object->head = head;
		object->look = look;
		object->dx = object->dy = object->position = object->var = 0;
		switch(type) {
			case DUCK: object->hitbox = object_newHitbox(2, 0, 13, 15); break;
			case EGG:
				switch(head) {
					case UP: object->hitbox = object_newHitbox(3, 15, 11, 15); break;
					case DOWN: object->hitbox = object_newHitbox(4, 0, 12, 0); break;
					case LEFT: object->hitbox = object_newHitbox(15, 4, 15, 12); break;
					case RIGHT: object->hitbox = object_newHitbox(0, 3, 0, 11); break;
				}
				break;
			case GRAVITY_SWITCH: object->hitbox = object_newHitbox(7, 7, 8, 8); break;
			case CUBE: object->hitbox = object_newHitbox(1, 1, 14, 14); break;
			case CATERPILLAR:
				switch(head) {
					case UP: object->hitbox = object_newHitbox(0, 8, 15, 15); break;
					case DOWN: object->hitbox = object_newHitbox(0, 0, 15, 7); break;
					case LEFT: object->hitbox = object_newHitbox(8, 0, 15, 15); break;
					case RIGHT: object->hitbox = object_newHitbox(0, 0, 7, 15); break;
				}
				break;
			case BULLET: object->hitbox = object_newHitbox(0, 0, 2, 2); break;
			case HOMING_BULLET: object->hitbox = object_newHitbox(0, 0, 1, 1); break;
			case PESTLE:
				switch(head) {
					case UP:
					case DOWN: object->hitbox = object_newHitbox(0, 0, 15, 31); break;
					case LEFT:
					case RIGHT: object->hitbox = object_newHitbox(0, 0, 31, 15); break;
				}
				break;
		}
	}
	return object;
}

int object_collision(Object* obj1, Object* obj2)
{
	Hitbox h1, h2;
	h1.x1 = obj1->x + obj1->hitbox.x1;
	h1.y1 = obj1->y + obj1->hitbox.y1;
	h1.x2 = obj1->x + obj1->hitbox.x2;
	h1.y2 = obj1->y + obj1->hitbox.y2;
	h2.x1 = obj2->x + obj2->hitbox.x1;
	h2.y1 = obj2->y + obj2->hitbox.y1;
	h2.x2 = obj2->x + obj2->hitbox.x2;
	h2.y2 = obj2->y + obj2->hitbox.y2;
	if(	((h2.x1 >= h1.x1 && h2.x1 <= h1.x2) || (h2.x2 >= h1.x1 && h2.x2 <= h1.x2)) &&
		((h2.y1 >= h1.y1 && h2.y1 <= h1.y2) || (h2.y2 >= h1.y1 && h2.y2 <= h1.y2)) )
		return 1;
	return 0;
}

/* private functions */
static Hitbox object_newHitbox(short x1, short y1, short x2, short y2)
{
	Hitbox hitbox;
	hitbox.x1 = x1;
	hitbox.x2 = x2;
	hitbox.y1 = y1;
	hitbox.y2 = y2;
	return hitbox;
}
