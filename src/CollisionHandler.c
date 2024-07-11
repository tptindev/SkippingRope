#include "CollisionHandler.h"
void EarthCollision(Entity* earth, Entity* other)
{
	if (earth->components.health != NULL)
	{
		earth->components.health->current-=20;
	}
}
void MoonCollision(Entity* moon, Entity* other)
{
	if (other->components.health != NULL)
	{
		other->components.health->current = 0;
	}
}
