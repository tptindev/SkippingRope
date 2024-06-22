#include "entity.h"
#include <stdlib.h>

Entity* create_entity(World2D* world)
{
	if (world->objId.last > world->objId.max) { return NULL; }
	Entity* entity = (Entity*)malloc(sizeof(Entity));
	entity->id = world->objId.last++;
	entity->components = CreateArray1D();
	return entity;
}

void add_component(Entity *entity, void* component)
{
	Array1DPush(entity->components, component);
}
