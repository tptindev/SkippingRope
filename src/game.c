#include "game.h"
#include <stdio.h>
#include "Physics2D/World2D.h"
#include "draw.h"
#include "/ecs/entity.h"
float world_scale = 80.0f;
void game_initialize(void* userdata)
{	
	World2D* world = CreateWorld(9.8, 5.0f, 3.0f);
	Entity* earth = create_entity(world);
	Entity* moon = create_entity(world);

	printf("%u %u \n", earth->id, moon->id);
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
