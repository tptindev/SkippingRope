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
	}
	return entity;
}

void FreeEntity(Entity* entity)
{
	if (entity->components.transform) FreeComponent(entity->components.transform);
	if (entity->components.collider->shape.define) FreeShape(entity->components.collider->shape.define);
	if (entity->components.collider) FreeComponent(entity->components.collider);
	if (entity->components.input) FreeComponent(entity->components.input);

	free(entity);
}

void FreeComponent(void* ptr)
{
	free(ptr);
	ptr = NULL;
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

void AddCircleColliderComponent(void* userdata, QuadTree* tree, Entity* entity, Vec2 offset, float radius)
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

