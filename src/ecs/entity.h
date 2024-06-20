#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "../Physics2D/World2D.h"
#include "../Physics2D/DataStructs/Array1D.h"
#include "component.h"

typedef struct {
	unsigned int id;
	Array1D* components;
} Entity;

Entity* create_entity(World2D* world);
void add_component(Entity* entity, void* component);

#endif // __ENTITY_H__
