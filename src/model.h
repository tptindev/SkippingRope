#include "pd_api.h"

typedef struct human
{	
	float x;
	float y;
	float z;
	float speed;
	float mass;
	LCDSprite** sprites;
} Human;

typedef struct rope
{
	float radus;
	float mass;
	int material;
	unsigned int length;
} Rope;