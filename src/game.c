#include "game.h"
#include "pd_api.h"
#include "Physics2D/DataStructs/QuadTree.h"
#include "Physics2D/World2D.h"
#include "ecs/entity.h"
#include "ecs/component.h"
#include "ecs/system.h"
#include "CollisionHandler.h"

float world_scale = 80.0f;
static PlaydateAPI* api = NULL;
static World2D* world = NULL;
static Entity* earth = NULL;
static Entity* moon = NULL;
static Entity* enemy = NULL;
static QuadTree* tree = NULL;

void game_initialize(void* userdata)
{	
	api = userdata;
	world = CreateWorld((Vec2) { 10.0f, 10.0f }, 5.0f, 3.0f);

	tree = CreateQuadTreeNode(NULL, world->w, world->h, 0);
	{ // earth
		earth = CreateEntity(world, (Vec2){ 2.5f, 1.5f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
		if (earth != NULL)
		{
			AddCircleColliderComponent(api, tree, earth, (Vec2) { 0.0f, 0.0f }, 0.3f);
		}
	}

	{ // moon
		moon = CreateEntity(world, (Vec2){ 2.0f, 1.0f }, (Vec2){ 0.0f, 0.0f }, (Vec2){ 1.0f, 1.0f });
		if (moon != NULL)
		{
			AddCircleColliderComponent(api, tree, moon, (Vec2) { 0.0f, 0.0f }, 0.15f);
			AddKeyInputComponent(api, moon, false, false, false, false, false, false, true);
		}
	}

	{ // enemy
		enemy = CreateEntity(world, (Vec2) { 0.0f, 0.0f }, (Vec2) { 0.0f, 0.0f }, (Vec2) { 1.0f, 1.0f });
		if (enemy != NULL)
		{
			if (earth != NULL)
			{
				enemy->components.motion->acceleration = world->gravity;
				enemy->components.motion->direction = Vec2Normalize(Vec2Subtract(earth->components.transform->position, enemy->components.transform->position));
			}
			AddCircleColliderComponent(api, tree, enemy, (Vec2) { 0.0f, 0.0f }, 0.05f);
		}
	}
}

void game_update(float dt)
{
	QuadTreeClear(tree);
	{ // earth
		UpdateInput(api, earth);
		UpdateScale(earth, 1);
		UpdatePosition(earth, (Vec2) { 0.0f, 0.0f }, dt);
		UpdateCollider(earth, tree);
	}
	{ // moon
		UpdateInput(api, moon);
		UpdateScale(moon, 1);
		UpdateRotation(moon, 0);
		UpdatePosition(moon, earth->components.transform->position, dt);
		UpdateCollider(moon, tree);
	}
	{ // enemy
		UpdateInput(api, enemy);
		UpdateScale(enemy, 1);
		UpdateCollider(enemy, tree);
		UpdateMovement(enemy, dt);
	}
	UpdateCollision(moon, tree, MoonCollision);
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{ // earth
		if (earth->components.collider != NULL)
		{
			Rect2D* box = &earth->components.collider->shape.box;
			Vec2* position = &earth->components.transform->position;
			int x = (int)(box->x * 80.0f);
			int y = (int)(box->y * 80.0f);
			int width = (int)(box->width * 80.0f);
			int height = (int)(box->height * 80.0f);
			api->graphics->drawRect(x, y, width, height, kColorBlack);
			api->graphics->drawEllipse((int)(position->x * 80.0f) - width / 2, (int)(position->y * 80.0f) - height / 2, width, height, 2, 0.0f, 0.0f, kColorBlack);
		}
	}

	{ // moon
		if (&moon->components.collider != NULL)
		{
			Rect2D* box = &moon->components.collider->shape.box;
			Vec2* position = &moon->components.transform->position;
			int x = (int)(box->x * 80.0f);
			int y = (int)(box->y * 80.0f);
			int width = (int)(box->width * 80.0f);
			int height = (int)(box->height * 80.0f);
			api->graphics->drawRect(x, y, width, height, kColorBlack);
			api->graphics->drawEllipse((int)(position->x * 80.0f) - width / 2, (int)(position->y * 80.0f) - height / 2, width, height, 2, 0.0f, 0.0f, kColorBlack);
		}
	}

	{ // enemy
		if (&enemy->components.collider != NULL)
		{
			Rect2D* box = &enemy->components.collider->shape.box;
			Vec2* position = &enemy->components.transform->position;
			int x = (int)(box->x * 80.0f);
			int y = (int)(box->y * 80.0f);
			int width = (int)(box->width * 80.0f);
			int height = (int)(box->height * 80.0f);
			api->graphics->drawRect(x, y, width, height, kColorBlack);
			api->graphics->drawEllipse((int)(position->x * 80.0f) - width / 2, (int)(position->y * 80.0f) - height / 2, width, height, 2, 0.0f, 0.0f, kColorBlack);
		}
	}
}

void game_destroy()
{
	FreeQuadTree(tree);
	DestroyWorld(world);
	FreeEntity(earth);
	FreeEntity(moon);
	FreeEntity(enemy);

}
