#include "pd_api.h"
typedef struct character
{	
	const char* name;
	const char** paths;
	LCDSprite** sprites;
	float x;
	float y;
	int z;
	float speed;
	float mass;
} Character;

typedef struct rope
{
	float radus;
	float mass;
	int material;
	unsigned int length;
} Rope;