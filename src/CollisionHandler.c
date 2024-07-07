#include "CollisionHandler.h"
#include "pd_api.h"
void MoonCollision(Entity* moon, Entity* other)
{
	if (other->components.health != NULL)
	{
		other->components.health->current = 0;
	}
}
