#include "system.h"
#include "entity.h"
#include "../Physics2D/Collision.h"
#include "pd_api.h"

void UpdateRotation(Entity* entity, float angle)
{
	if (entity == NULL) return;
}

void UpdateMovement(Entity* entity, float dt)
{
	if (entity == NULL) return;
	if (entity->components.transform != NULL && entity->components.motion != NULL)
	{
		entity->components.motion->acceleration.x += 0.1f;
		entity->components.motion->acceleration.y += 0.1f;

		Vec2 velocity = Vec2Subtract(entity->components.transform->position, entity->components.motion->last_position);
		entity->components.transform->position.x += velocity.x + (entity->components.motion->direction.x * entity->components.motion->acceleration.x) * dt * dt;
		entity->components.transform->position.y += velocity.y + (entity->components.motion->direction.y * entity->components.motion->acceleration.y) * dt * dt;
	}
}

void UpdatePosition(Entity* entity, Vec2 buffer, float dt)
{
	if (entity == NULL) return;
	if (entity->components.transform != NULL)
	{
		entity->components.transform->position.x += buffer.x;
		entity->components.transform->position.y += buffer.y;
	}
}

void UpdateCollider(Entity* entity, struct QuadTree* tree)
{
	if (entity == NULL) return;
	if (entity->components.collider != NULL && entity->components.transform != NULL)
	{
		// Update Collider
		if (entity->components.collider->shape.type == CIRCLE)
		{
			Circle* circle = entity->components.collider->shape.define;
			circle->center = Vec2Add(entity->components.transform->position, entity->components.collider->offset);
			entity->components.collider->shape.box.x = entity->components.collider->offset.x + entity->components.transform->position.x - circle->radius;
			entity->components.collider->shape.box.y = entity->components.collider->offset.y + entity->components.transform->position.y - circle->radius;
		}

		QuadtreeInsert(tree, entity, &entity->components.collider->shape.box);
	}
}

void UpdateCollision(Entity* entity, struct QuadTree* tree, void (*callback)(Entity* a, Entity* b))
{
	if (entity == NULL) return;
	Array1D* nodes = CreateArray1D();
	QuadTreeSearch(tree, nodes, &entity->components.collider->shape.box);
	Circle* c0 = entity->components.collider->shape.define;
	for (int i = 0; i < nodes->size; i++)
	{
		struct QuadTree* _node = Array1DItemAtIndex(nodes, i);
		Array1D* _objs = _node->objects;
		for (int j = 0; j < _node->objects->size; ++j)
		{
			Entity* other = Array1DItemAtIndex(_objs, j);
			if (other == NULL) continue;
			if (entity->id != other->id)
			{
				Circle* cx = other->components.collider->shape.define;

				bool collided = IsCollisionCircle(c0, cx);
				if (collided == true)
				{
					callback(entity, other);
				}
			}
		}
	}
}

void UpdateSprite(Entity* entity, unsigned int tick)
{
	if (entity == NULL) return;
	if (entity->components.sprite != NULL)
	{
	}
}

void UpdateAnimateSprite(Entity* entity, unsigned int tick)
{
	if (entity == NULL) return;
	if (entity->components.animated_sprite != NULL)
	{
		int frame_count = entity->components.animated_sprite->frame_count;
		entity->components.animated_sprite->frame_index = (tick % frame_count);
	}
}

void UpdateHealth(void* userdata, Entity** entity, void (*callback)(void* api, Entity** entity, void* health))
{

	if (entity == NULL || userdata == NULL) return;
	if (*entity == NULL) return;
	if ((*entity)->components.health != NULL && callback != NULL)
	{
		callback(userdata, entity, (*entity)->components.health);
	}
}

void UpdateRenderer(void* userdata, Entity* entity)
{
	if (entity == NULL || userdata == NULL) return;
	PlaydateAPI* api = userdata;
	LCDSprite* sprite = NULL;
	LCDBitmap* bitmap = NULL;
	int16_t z_order = -1;
	if (entity->components.sprite != NULL)
	{
		sprite = entity->components.sprite->_ptr;
		z_order = entity->components.sprite->order_in_layer;
		bitmap = entity->components.sprite->bitmap;
	} 
	else if (entity->components.animated_sprite != NULL)
	{
		sprite = entity->components.animated_sprite->_ptr;
		z_order = entity->components.animated_sprite->order_in_layer;
		bitmap = entity->components.animated_sprite->bitmaps[entity->components.animated_sprite->frame_index];
		api->sprite->setImage(sprite, bitmap, kBitmapUnflipped);
	}

	if (sprite != NULL && bitmap != NULL && entity->components.transform != NULL)
	{
		api->sprite->setZIndex(sprite, z_order);
		api->sprite->moveTo(sprite, entity->components.transform->position.x * 80.0f, entity->components.transform->position.y * 80.0f);
		api->sprite->addSprite(sprite);
	}
}

void UpdateInput(void* userdata, Entity* entity)
{
	if (entity == NULL || userdata == NULL) return;
	PlaydateAPI* api = userdata;
	if (entity->components.motion != NULL)
	{
		entity->components.motion->last_position.x = entity->components.transform->position.x;
		entity->components.motion->last_position.y = entity->components.transform->position.y;
	}
	if (entity->components.input != NULL)
	{
		if (entity->components.input->crank == true && entity->components.transform != NULL) 
		{
			double angle_rad = api->system->getCrankAngle() * (3.14159265358979323846f / 180.0f);
			entity->components.transform->position.x = (float)(0.5f * cos(angle_rad));
			entity->components.transform->position.y = (float)(0.5f * sin(angle_rad));
		} 
		// ...
	}
}

void UpdateScale(Entity* entity, float scale)
{
	if (entity == NULL) return;
}
