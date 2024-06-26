#include "entity.h"
#include "component.h"
#include <stdlib.h>

Entity* CreateEntity(World2D* world, Vec2 position, Vec2 rotation, Vec2 scale)
{
	if (world == NULL) return NULL;
	if (world->objId.last > world->objId.max) { return NULL; }
	Entity* entity = (Entity*)malloc(sizeof(Entity));
	if (entity) {
		entity->id = world->objId.last++;
		Transform* transform = (Transform*)malloc(sizeof(Transform));
		if (transform) 
		{
			// Position
			transform->position.x = position.x;
			transform->position.y = position.y;
			
			// Rotation
			transform->rotation.x = rotation.x;
			transform->rotation.y = rotation.y;

			// Scale
			transform->scale.x = scale.x;
			transform->scale.y = scale.y;
		}
	}
	return entity;
}

void FreeEntity(Entity* entity)
{
	if (entity->components.transform) free(entity->components.transform);
	free(entity);
}

