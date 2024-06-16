#include "game.h"
#include "draw.h"
#include "struct_defines.h"
#include "Physics2D/DataStructs/Array1D.h"
float world_scale = 80.0f;
static Array1D* objs = NULL;
void game_initialize(void* userdata)
{	
	objs = CreateArray1D();
}

void game_update(float deltatime)
{
}

void game_draw()
{
}

void game_destroy()
{
}
