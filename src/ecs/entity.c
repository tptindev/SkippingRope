#include "entity.h"
#include "component.h"
#include <stdlib.h>
#include <stdio.h>
#include "pd_api.h"
#include "../Utils.h"
Entity* CreateEntity(World2D* world, Vec2 position, Vec2 rotation, Vec2 scale)
{
	if (world == NULL) return NULL;
	if (world->objId.last > world->objId.max) { return NULL; }
	Entity* entity = malloc(sizeof(Entity));
	if (entity != NULL) {
		entity->id = world->objId.last++;
		entity->components.transform = malloc(sizeof(Transform));
		if (entity->components.transform != NULL)
		{
			// Position
			entity->components.transform->position.x = position.x;
			entity->components.transform->position.y = position.y;
			
			// Rotation
			entity->components.transform->rotation.x = rotation.x;
			entity->components.transform->rotation.y = rotation.y;

			// Scale
			entity->components.transform->scale.x = scale.x;
			entity->components.transform->scale.y = scale.y;
		}

		entity->components.motion = malloc(sizeof(Motion));
		if (entity->components.motion != NULL)
		{
			entity->components.motion->acceleration.x = 0.0f;
			entity->components.motion->acceleration.y = 0.0f;

			entity->components.motion->last_position.x = 0.0f;
			entity->components.motion->last_position.y = 0.0f;

			entity->components.motion->direction.x = 0.0f;
			entity->components.motion->direction.y = 0.0f;
		}

		entity->components.collider = NULL;
		entity->components.input = NULL;
		entity->components.sprite = NULL;
		entity->components.animated_sprite = NULL;
	}
	return entity;
}

void DestroyEntity(void *userdata, Entity* entity)
{
	FreeEntity(userdata, entity);
}

void FreeEntity(void *api, Entity* entity)
{
	if (entity == NULL || api == NULL) return;
	if (entity->components.transform != NULL) FreeComponent(entity->components.transform);
	if (entity->components.collider != NULL)
	{
		FreeShape(entity->components.collider->shape.define);
		FreeComponent(entity->components.collider);
	}
	if (entity->components.input != NULL) FreeComponent(entity->components.input);
	if (entity->components.sprite != NULL)
	{
		freeBitmap(api, entity->components.sprite->bitmap);
		freeSprite(api, entity->components.sprite->_ptr);
		FreeComponent(entity->components.sprite);
	}
	if (entity->components.animated_sprite != NULL)
	{
		freeSprite(api, entity->components.animated_sprite->_ptr);
		for (int i = 0; i < entity->components.animated_sprite->frame_count; i++)
		{
			freeBitmap(api, entity->components.animated_sprite->bitmaps[i]);
		}
		freeObjPtr(entity->components.animated_sprite->bitmaps);
		FreeComponent(entity->components.animated_sprite);
	}
	freeObjPtr(entity);
}

void FreeComponent(void* ptr)
{
	freeObjPtr(ptr);
}

void AddAnimatedSpriteComponent(void* userdata, Entity* entity, const char* source, int frame_width, int frame_height, int frame_count, int16_t z_order)
{
	PlaydateAPI* api = userdata;
	entity->components.animated_sprite = malloc(sizeof(AnimatedSprite));
	if (entity->components.animated_sprite != NULL && entity->components.transform != NULL)
	{
		entity->components.animated_sprite->source = source;
		entity->components.animated_sprite->frame_index = 0;
		entity->components.animated_sprite->frame_width = frame_width;
		entity->components.animated_sprite->frame_height = frame_height;
		entity->components.animated_sprite->frame_count = frame_count;
		entity->components.animated_sprite->order_in_layer = z_order;
		entity->components.animated_sprite->bitmaps = NULL;
		entity->components.animated_sprite->_ptr = NULL;

		void* (*bitmaps) = malloc(sizeof(void*) * frame_count);
		if (bitmaps != NULL)
		{
			for (int i = 0; i < frame_count; i++)
			{
				char path_buffer[32];
				snprintf(path_buffer, sizeof(path_buffer), "images/enemy/%d.png", i);

				const char* outerr = NULL;
				bitmaps[i] = api->graphics->loadBitmap(path_buffer, outerr); // bitmaps[i] <=> *(bitmaps + i)

				if (outerr != NULL)
				{
					api->system->logToConsole("Error: %s", outerr);
					api->graphics->freeBitmap(bitmaps[i]);
					bitmaps = NULL;
					return;
				}
			}
		}

		entity->components.animated_sprite->bitmaps = bitmaps;
		LCDSprite* sprite_ptr = api->sprite->newSprite();
		if (sprite_ptr != NULL && bitmaps != NULL)
		{
			entity->components.animated_sprite->_ptr = sprite_ptr;
		}
		else
		{
			api->sprite->freeSprite(sprite_ptr);
		}
	}
}

void AddSpriteComponent(void* userdata, Entity* entity, const char* source, bool flip, int16_t z_order)
{
	PlaydateAPI* api = userdata;
	entity->components.sprite = malloc(sizeof(Sprite));
	if (entity->components.sprite != NULL && entity->components.transform != NULL)
	{
		entity->components.sprite->source = source;
		entity->components.sprite->flip = flip;
		entity->components.sprite->order_in_layer = z_order;
		entity->components.sprite->bitmap = NULL;
		entity->components.sprite->_ptr = NULL;

		const char* outerr = NULL;
		LCDBitmap* bitmap_ptr = api->graphics->loadBitmap(source, &outerr);
		if (outerr != NULL)
		{
			api->system->logToConsole("Error: %s", outerr);
			api->graphics->freeBitmap(bitmap_ptr);
			return;
		}
		entity->components.sprite->bitmap = bitmap_ptr;
		LCDSprite* sprite_ptr = api->sprite->newSprite();
		if (sprite_ptr != NULL)
		{
			entity->components.sprite->_ptr = sprite_ptr;
			api->sprite->setImage(sprite_ptr, bitmap_ptr, kBitmapUnflipped);
			api->sprite->setZIndex(sprite_ptr, z_order);
			api->sprite->moveTo(
				sprite_ptr,
				entity->components.transform->position.x * 80.0f,
				entity->components.transform->position.y * 80.0f
			);
		}
		else
		{
			api->sprite->freeSprite(sprite_ptr);
		}
	}
}

void AddKeyInputComponent(void* userdata, Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank)
{
	entity->components.input = malloc(sizeof(KeyInput));
	if (entity->components.input != NULL)
	{
		entity->components.input->left = left;
		entity->components.input->right = right;
		entity->components.input->up = up;
		entity->components.input->down = down;
		entity->components.input->a = a;
		entity->components.input->b = b;
		entity->components.input->crank = crank;
	}
}

void AddCircleColliderComponent(void* userdata, struct QuadTree* tree, Entity* entity, Vec2 offset, float radius)
{
	PlaydateAPI* api = userdata;
	entity->components.collider = malloc(sizeof(Collider));
	if (entity->components.collider != NULL && entity->components.transform != NULL)
	{
		entity->components.collider->offset.x = offset.x;
		entity->components.collider->offset.y = offset.y;
		entity->components.collider->shape.type = CIRCLE;
		entity->components.collider->shape.define = CreateCircle(
			Vec2Add(entity->components.transform->position, offset),
			radius
		);
		GetCircleBoundary(&entity->components.collider->shape.box, entity->components.collider->shape.define);
		QuadtreeInsert(tree, entity, &entity->components.collider->shape.box);
	}
}

