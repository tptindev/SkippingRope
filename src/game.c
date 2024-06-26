#include "game.h"
#include "pd_api.h"
#include "Physics2D/DataStructs/QuadTree.h"
#include "Physics2D/World2D.h"
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
		earth = CreateEntity(world, (Vec2){ 2.5f, 1.5f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
		if (earth != NULL)
		{
		}
	}

	{ // moon
		moon = CreateEntity(world, (Vec2){ 2.2f, 1.2f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
		if (moon != NULL)
		{
		}
	}
}

void game_update(float dt)
{
	{ // moon
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{ // earth
		Vec2 *position = &earth->components.transform->position;
		api->graphics->drawEllipse(position->x, position->y, 24, 24, 2, 0.0f, 0.0f, kColorBlack);
	}

	{ // moon
		Vec2 *position = &moon->components.transform->position;
		api->graphics->drawEllipse(position->x, position->y, 8, 8, 2, 0.0f, 0.0f, kColorBlack);
	}
}

void game_destroy()
{
	QuadTreeClear(tree);
	DestroyWorld(world);
	FreeEntity(earth);
	FreeEntity(moon);
}
