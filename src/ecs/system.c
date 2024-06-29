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

void UpdateCollider(void* userdata, Entity* entity, struct QuadTree* tree)
{
	PlaydateAPI* api = userdata;
	//QuadTreeClear(tree);
	if (entity->components.collider != NULL && entity->components.transform != NULL)
	{
		// Update Collider
		if (entity->components.collider->shape.type == CIRCLE)
		{
			Circle* cirle = entity->components.collider->shape.define;
			entity->components.collider->shape.box.x = entity->components.collider->offset.x + entity->components.transform->position.x - cirle->radius;
			entity->components.collider->shape.box.y = entity->components.collider->offset.y + entity->components.transform->position.y - cirle->radius;
		}

		//QuadtreeInsert(tree, entity, &entity->components.collider->shape.box);
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
