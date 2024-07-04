#include "entity.h"
#include "component.h"
#include <stdlib.h>
#include "pd_api.h"

Entity* CreateEntity(World2D* world, Vec2 position, Vec2 rotation, Vec2 scale)
{
	if (world == NULL) return NULL;
	if (world->objId.last > world->objId.max) { return NULL; }
	Entity* entity = (Entity*)malloc(sizeof(Entity));
	if (entity != NULL) {
		entity->id = world->objId.last++;
		entity->components.transform = (Transform*)malloc(sizeof(Transform));
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

		entity->components.motion = (Motion*)malloc(sizeof(Motion));
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
		entity->components.animation_sprite = NULL;
	}
	return entity;
}

void FreeEntity(Entity* entity)
{
	if (entity->components.transform != NULL) FreeComponent(entity->components.transform);
	if (entity->components.collider != NULL)
	{
		FreeShape(entity->components.collider->shape.define);
		FreeComponent(entity->components.collider);
	}
	if (entity->components.input != NULL) FreeComponent(entity->components.input);
	if (entity->components.sprite != NULL)
	{
		free(entity->components.sprite->bitmap);
		free(entity->components.sprite->_ptr);
		FreeComponent(entity->components.sprite);
	}
	if (entity->components.animation_sprite != NULL) FreeComponent(entity->components.animation_sprite);
	free(entity);
}

void FreeComponent(void* ptr)
{
	free(ptr);
	ptr = NULL;
}

void AddAnimatedSpriteComponent(void* userdata, Entity* entity, const char* s, int fx, int fy, int fw, int fh, float fr, int fd, int fc, int16_t z_order)
{
	entity->components.animation_sprite = (AnimatedSprite*)malloc(sizeof(AnimatedSprite));
	if (entity->components.animation_sprite != NULL && entity->components.transform != NULL)
	{
		entity->components.animation_sprite->frame_x = fx;
		entity->components.animation_sprite->frame_y = fy;
		entity->components.animation_sprite->frame_width = fw;
		entity->components.animation_sprite->frame_height = fh;
		entity->components.animation_sprite->frame_rate = fr;
		entity->components.animation_sprite->frame_duration = fd;
		entity->components.animation_sprite->frame_count = fc;
		entity->components.animation_sprite->source = s;
		entity->components.animation_sprite->order_in_layer = z_order;
	}
}

void AddSpriteComponent(void* userdata, Entity* entity, const char* source, bool flip, int16_t z_order)
{
	PlaydateAPI* api = userdata;
	entity->components.sprite = (Sprite*)malloc(sizeof(Sprite));
	if (entity->components.sprite != NULL && entity->components.transform != NULL)
	{
		entity->components.sprite->source = source;
		entity->components.sprite->flip = flip;
		entity->components.sprite->order_in_layer = z_order;

		const char* outerr = NULL;
		LCDBitmap* bitmap_ptr = api->graphics->loadBitmap(entity->components.sprite->source, &outerr);
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
			api->sprite->moveTo(sprite_ptr, entity->components.transform->position.x * 80.0f, entity->components.transform->position.y * 80.0f);
			api->sprite->addSprite(sprite_ptr);
		}
	}
}

void AddKeyInputComponent(void* userdata, Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank)
{
	entity->components.input = (KeyInput*)malloc(sizeof(KeyInput));
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
	entity->components.collider = (Collider*)malloc(sizeof(Collider));
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

