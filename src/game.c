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
			AddCircleColliderComponent(earth, tree, (Vec2) { 0.0f, 0.0f }, 0.5f);
		}
	}

	{ // moon
		moon = CreateEntity(world);
		if (moon != NULL)
		{
			AddRegidbodyComponent(moon, e_kinematic, 150, 1);
			AddTransformComponent(moon, 2.5f, 1.5f, 0, 1);
			AddHealthComponent(moon, 100, 100);
			AddSpriteComponent(moon, "images/moon.png");
			AddKeyInputComponent(moon, false, false, false, false, false, false, true);
			AddCircleColliderComponent(moon, tree, (Vec2) { 0.0f, 0.0f }, 0.2f);
		}
	}
}

void game_update(float dt)
{
	{ // moon
		UpdatePosition(api, moon->components.transform, moon->components.input, dt);
		moon->components.transform->x += earth->components.transform->x;
		moon->components.transform->y += earth->components.transform->y;

		UpdateCollision(api, moon->components.collider, tree);
	}
}

void game_draw()
{
	api->graphics->clear(kColorWhite);
	api->graphics->setBackgroundColor(kColorBlack);

	{ // earth
		Circle* c = earth->components.collider->shape.define;
		api->graphics->drawEllipse((int)(c->center.x * 80.0f), (int)(c->center.y * 80.0f), c->radius * 80.0f, c->radius * 80.0f, 2, 0, 0, kColorBlack);
		UpdateSprite(api, earth->components.sprite, earth->components.transform);
	}

	{ // moon
		Circle* c = moon->components.collider->shape.define;
		api->graphics->drawEllipse((int)(c->center.x * 80.0f), (int)(c->center.y * 80.0f), c->radius * 80.0f, c->radius * 80.0f, 2, 0, 0, kColorBlack);
		UpdateSprite(api, moon->components.sprite, moon->components.transform);
	}
}

void game_destroy()
{
	QuadTreeClear(tree);
	DestroyWorld(world);
	FreeEntity(earth);
	FreeEntity(moon);
}
