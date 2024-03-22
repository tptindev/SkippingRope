#ifndef __MODEL_H__
#define __MODEL_H__

#include "pd_api.h"

typedef struct character
{
	const char* id;
	const char** paths;
	LCDSprite** sprites;
	size_t capacity;
} Character;

typedef struct rope
{
	float radus;
	float mass;
	int material;
	unsigned int length;
} Rope;

#endif // __MODEL_H__