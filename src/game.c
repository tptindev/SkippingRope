#include "game.h"
#include <stdio.h>
#include "Physics2D/World2D.h"
#include "draw.h"
#include "ecs/entity.h"
#include "ecs/component.h"

static PlaydateAPI* api = NULL;
float world_scale = 80.0f;
static World2D* earth = NULL;
static Entity* moon = NULL;
void game_initialize(void* userdata)
{	
	api = userdata;
	World2D* world = CreateWorld(9.8, 5.0f, 3.0f);

	earth = CreateEntity(world);
	AddPositionComponent(earth, 2.5f, 1.5f);
	AddHealthComponent(earth, 100, 100);

	moon = CreateEntity(world);
	AddPositionComponent(moon, 1.5f, 1.5f);
	AddHealthComponent(moon, 100, 100);
}

void game_update(float deltatime)
{
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);
}

void game_destroy()
{
	FreeEntity(earth);
	FreeEntity(moon);
}
