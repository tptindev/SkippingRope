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
	world = CreateWorld(9.8f, 5.0f, 3.0f);
	tree = CreateQuadTreeNode(NULL, world->w, world->h, 0);
	{ // earth
		earth = CreateEntity(world, (Vec2){ 2.5f, 1.5f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
		if (earth != NULL)
		{
			AddCircleColliderComponent(api, earth, (Vec2) { -0.1575f, -0.1575f }, 0.35f);
		}
	}

	{ // moon
		moon = CreateEntity(world, (Vec2){ 2.2f, 1.2f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
		if (moon != NULL)
		{
			AddCircleColliderComponent(api, moon, (Vec2) { -0.075f, -0.075f }, 0.15f);
			AddKeyInputComponent(api, moon, false, false, false, false, false, false, true);
		}
	}
}

void game_update(float dt)
{
	{ // earth
		UpdateInput(api, earth);
		UpdateScale(earth, 1);
		UpdatePosition(earth, (Vec2) { 0.0f, 0.0f }, dt);
		UpdateCollider(api, earth, tree);
	}
	{ // moon
		UpdateInput(api, moon);
		UpdateScale(moon, 1);
		UpdateRotation(moon, 0);
		UpdatePosition(moon, earth->components.transform->position, dt);
		UpdateCollider(api, moon, tree);
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{ // earth
		if (earth->components.collider != NULL)
		{
			Rect2D* box = &earth->components.collider->shape.box;
			api->graphics->drawRect(
				(int)(box->x * 80.0f),
				(int)(box->y * 80.0f),
				(int)(box->width * 80.0f),
				(int)(box->height * 80.0f),
				kColorBlack
			);
			api->graphics->drawEllipse(
				(int)(box->x * 80.0f),
				(int)(box->y * 80.0f),
				(int)(box->width * 80.0f),
				(int)(box->height * 80.0f),
				2,
				0.0f,
				0.0f,
				kColorBlack
			);
		}
	}

	{ // moon
		if (&moon->components.collider != NULL)
		{
			Rect2D* box = &moon->components.collider->shape.box;
			api->graphics->drawRect(
				(int)(box->x * 80.0f),
				(int)(box->y * 80.0f),
				(int)(box->width * 80.0f),
				(int)(box->height * 80.0f),
				kColorBlack
			);
			api->graphics->drawEllipse(
				(int)(box->x * 80.0f),
				(int)(box->y * 80.0f),
				(int)(box->width * 80.0f),
				(int)(box->height * 80.0f),
				2,
				0.0f,
				0.0f,
				kColorBlack
			);
		}
	}
}

void game_destroy()
{
	FreeQuadTree(tree);
	DestroyWorld(world);
	FreeEntity(earth);
	FreeEntity(moon);
}
