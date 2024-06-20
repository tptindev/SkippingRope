#include "entity.h"
#include <stdlib.h>

Entity* create_entity(World2D* world)
{
	Entity* entity = (Entity*)malloc(sizeof(Entity));
	return entity;
}

void add_component(Entity *entity, void* component)
{
	Array1DPush(entity->components, component);
}
