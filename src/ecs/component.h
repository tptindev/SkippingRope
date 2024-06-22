#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <stddef.h>

typedef struct {
	float x;
	float y;
} Position;

typedef struct {
	float current;
	float max;
} Health;

typedef struct {
	const char* source;
	float degree;
} Sprite;

typedef struct {
	Position* position;
	Health* health;
	Sprite* sprite;
} Components;

#endif // __COMPONENT_H__
