#include "system.h"
#include "entity.h"
#include "../Physics2D/Collision.h"
#include "pd_api.h"

void UpdateRotation(Entity* entity, float angle)
{
}

void UpdatePosition(Entity* entity, Vec2 to, float dt)
{
	if (entity->components.transform != NULL)
	{
		entity->components.transform->position.x += to.x;
		entity->components.transform->position.y += to.y;
	}
}

void UpdateCollider(void* userdata, Entity* entity, QuadTree* tree)
{
	PlaydateAPI* api = userdata;
	if (entity->components.collider != NULL && entity->components.transform != NULL)
	{
		// Update Collider
		if (entity->components.collider->shape.type == CIRCLE)
		{
			Circle* cirle = entity->components.collider->shape.define;
			entity->components.collider->shape.box.x = entity->components.collider->offset.x + entity->components.transform->position.x - cirle->radius;
			entity->components.collider->shape.box.y = entity->components.collider->offset.y + entity->components.transform->position.y - cirle->radius;
		}

		QuadtreeInsert(tree, entity, &entity->components.collider->shape.box);
	}
}

void UpdateCollision(void* userdata, Collider* collider, QuadTree* origin)
{
	PlaydateAPI* api = userdata;
	Array1D* nodes = CreateArray1D();
	QuadTreeSearch(origin, nodes, &collider->shape.box);

	for (int i = 0; i < nodes->size; i++)
	{
		QuadTree* _node = (struct QuadTree*)Array1DItemAtIndex(nodes, i);
		Array1D* _objs = _node->objects;
		for (int j = 0; j < _node->objects->size - 1; ++j) {
			for (int k = 1; k < _node->objects->size; k++) {
				Entity* obj1 = (Entity*)Array1DItemAtIndex(_objs, j);
				Entity* obj2 = (Entity*)Array1DItemAtIndex(_objs, k);

				Circle* c1 = obj1->components.collider->shape.define;
				Circle* c2 = obj2->components.collider->shape.define;

				bool collided = IsCollisionCircle(c1, c2);
				if (collided)
				{
					api->system->logToConsole("%f %f %f - %f %f %f", c1->center.x, c2->center.y, c1->radius, c2->center.x, c2->center.y, c2->radius);
				}
			}
		}
	}
}

void UpdateInput(void* userdata, Entity* entity)
{
	PlaydateAPI* api = userdata;
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
