#include "CollisionHandler.h"

void MoonCollision(Entity* moon, Entity* other)
{
	other->components.motion->direction = (Vec2){ -1, -1 };
}
