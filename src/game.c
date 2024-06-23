#include "game.h"
#include <stdio.h>
#include "Physics2D/DataStructs/QuadTree.h"
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
static struct QuadTree* tree = NULL;

void game_initialize(void* userdata)
{	
	api = userdata;
	world = CreateWorld(9.8, 5.0f, 3.0f);
	tree = CreateQuadTreeNode(NULL, world->w, world->h, 0);
	{ // earth
		earth = CreateEntity(world);
		if (earth != NULL)
		{
			AddRegidbodyComponent(earth, e_static, 1000, 1);
			AddTransformComponent(earth, 2.5f, 1.5f, 0, 1);
			AddHealthComponent(earth, 100, 100);
			AddSpriteComponent(earth, "images/earth.png");
		}
	}

	{ // moon
		moon = CreateEntity(world);
		if (moon != NULL)
		{
			AddRegidbodyComponent(moon, e_kinematic, 150, 1);
			AddTransformComponent(moon, 1.5f, 1.5f, 0, 1);
			AddHealthComponent(moon, 100, 100);
			AddSpriteComponent(moon, "images/moon.png");
			AddKeyInputComponent(moon, false, false, false, false, false, false, true);
		}
	}

}

void game_update(float dt)
{
	{ // moon
		UpdatePosition(api, moon->component.transform, moon->component.input, dt);
		moon->component.transform->x += earth->component.transform->x;
		moon->component.transform->y += earth->component.transform->y;
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{ // earth
		UpdateSprite(api, earth->component.sprite, earth->component.transform);
	}

	{ // moon
		UpdateSprite(api, moon->component.sprite, moon->component.transform);
	}
}

void game_destroy()
{
	QuadTreeClear(tree);
	DestroyWorld(world);
	FreeEntity(earth);
	FreeEntity(moon);
}
