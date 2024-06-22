#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <stddef.h>

struct Position {
	unsigned int entity_id;
	float x;
	float y;
};

struct Health {
	unsigned int entity_id;
	float current_health;
	float max_health;
};

struct Sprite {

};

#endif // __COMPONENT_H__
