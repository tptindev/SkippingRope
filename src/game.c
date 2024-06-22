#include "game.h"
#include <stdio.h>
#include "Physics2D/World2D.h"
#include "draw.h"
#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/system.h"

float world_scale = 80.0f;
static PlaydateAPI* api = NULL;
static World2D* world = NULL;
static Entity* earth = NULL;
static Entity* moon = NULL;
void game_initialize(void* userdata)
{	
	api = userdata;
	world = CreateWorld(9.8, 5.0f, 3.0f);

	{
		earth = CreateEntity(world);
		if (earth != NULL)
		{
			AddPositionComponent(earth, 2.5f, 1.5f);
			AddHealthComponent(earth, 100, 100);
			AddSpriteComponent(earth, "images/earth.png", 0);
		}
	}

	{
		moon = CreateEntity(world);
		if (moon != NULL)
		{
			AddPositionComponent(moon, 1.5f, 1.5f);
			AddHealthComponent(moon, 100, 100);
			AddSpriteComponent(moon, "images/moon.png", 0);

		}
	}

}

void game_update(float deltatime)
{
	{
		UpdatePosition(api, moon->component.position, deltatime);
		moon->component.position->x += earth->component.position->x;
		moon->component.position->y += earth->component.position->y;
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{
		UpdateSprite(api, earth->component.sprite, earth->component.position);
	}

	{
		UpdateSprite(api, moon->component.sprite, moon->component.position);
	}
}

void game_destroy()
{
	FreeEntity(earth);
	FreeEntity(moon);
}
