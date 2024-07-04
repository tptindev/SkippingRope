#include "system.h"
#include "entity.h"
#include "../Physics2D/Collision.h"
#include "pd_api.h"

void UpdateRotation(Entity* entity, float angle)
{
}

void UpdateMovement(Entity* entity, float dt)
{
	if (entity->components.transform != NULL && entity->components.motion != NULL)
	{
		entity->components.motion->acceleration.x += 0.1f;
		entity->components.motion->acceleration.y += 0.1f;

		Vec2 velocity = Vec2Subtract(entity->components.transform->position, entity->components.motion->last_position);
		entity->components.transform->position.x += velocity.x + (entity->components.motion->direction.x * entity->components.motion->acceleration.x) * dt * dt;
		entity->components.transform->position.y += velocity.y + (entity->components.motion->direction.y * entity->components.motion->acceleration.y) * dt * dt;
	}
}

void UpdatePosition(Entity* entity, Vec2 to, float dt)
{
	if (entity->components.transform != NULL)
	{
		entity->components.transform->position.x += to.x;
		entity->components.transform->position.y += to.y;
	}
}

void UpdateCollider(Entity* entity, QuadTree* tree)
{
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

void UpdateCollision(Entity* entity, QuadTree* tree, void (*callback)(Entity* a, Entity* b))
{
	if (entity != NULL)
	{
		Array1D* nodes = CreateArray1D();
		QuadTreeSearch(tree, nodes, &entity->components.collider->shape.box);
		Circle* c0 = entity->components.collider->shape.define;
		for (int i = 0; i < nodes->size; i++)
		{
			QuadTree* _node = Array1DItemAtIndex(nodes, i);
			Array1D* _objs = _node->objects;
			for (int j = 0; j < _node->objects->size; ++j) 
			{
				Entity* other = Array1DItemAtIndex(_objs, j);
				if (entity->id != other->id)
				{
					Circle* cx = other->components.collider->shape.define;

					bool collided = IsCollisionCircle(c0, cx);
					if (collided)
					{
						callback(entity, other);
					}
				}
			}
		}
	}
}

void UpdateRenderer(void* userdata, Entity* entity, Array1D* sprites)
{
	PlaydateAPI* api = userdata;
	if (entity->components.sprite_renderer != NULL && entity->components.transform != NULL)
	{
		const char* outerr = NULL;
		LCDBitmap* bitmap_ptr = api->graphics->loadBitmap(entity->components.sprite_renderer->source, &outerr);
		if (outerr != NULL)
		{
			api->system->logToConsole("Error: %s", outerr);
			api->graphics->freeBitmap(bitmap_ptr);
			return;
		}
		LCDSprite* sprite_ptr = api->sprite->newSprite();
		if (sprite_ptr != NULL)
		{
			api->sprite->setImage(sprite_ptr, bitmap_ptr, kBitmapUnflipped);
			api->sprite->setZIndex(sprite_ptr, entity->components.sprite_renderer->order_in_layer);
			api->sprite->moveTo(sprite_ptr, entity->components.transform->position.x * 80.0f, entity->components.transform->position.y * 80.0f);
			api->sprite->addSprite(sprite_ptr);
			Array1DPush(sprites, sprite_ptr);
		}
	}
}

void UpdateInput(void* userdata, Entity* entity)
{
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
}
