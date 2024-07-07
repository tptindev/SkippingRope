#include "CollisionHandler.h"
#include "pd_api.h"
void MoonCollision(void* userdata, World2D* world, Entity* moon, Entity* other)
{
	if (userdata == NULL) return;
	DestroyEntity(userdata, other);
}
