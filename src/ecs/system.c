#include "system.h"
#include "../draw.h"
#include "../Physics2D/Collision.h"
#include "entity.h"

void UpdatePosition(PlaydateAPI* api, Transform* trans, KeyInput* keyinput, float dt)
{
	if (keyinput != NULL)
	{
		if (keyinput->crank)
		{
			double angle_rad = api->system->getCrankAngle() * (3.14159265358979323846f / 180.0f);
			trans->x = (float)(0.9f * cos(angle_rad));
			trans->y = (float)(0.7f * sin(angle_rad));
		}
	}
}

void UpdateCollision(PlaydateAPI* api, BoxCollider* collider, struct QuadTree* origin)
{
	Array1D* nodes = CreateArray1D();
	QuadTreeSearch(origin, nodes, &collider->shape.box);
	for (int i = 0; i < nodes->size; i++)
	{
		struct QuadTree* _node = (struct QuadTree*)Array1DItemAtIndex(nodes, i);
		Array1D* _objs = _node->objects;
		for (int j = 0; j < _node->objects->size - 1; ++j) {
			for (int k = 1; k < _node->objects->size; k++) {
				Entity* obj1 = (Entity*)Array1DItemAtIndex(_objs, j);
				Entity* obj2 = (Entity*)Array1DItemAtIndex(_objs, k);
				
				Circle* c1 = (Circle*)obj1->components.collider->shape.define;
				Circle* c2 = (Circle*)obj2->components.collider->shape.define;

				bool collided = IsCollisionCircle(c1, c2);
				if (collided)
				{
					api->system->logToConsole("%f %f %f - %f %f %f",c1->center.x, c1->center.y, c1->radius, c2->center.x, c2->center.y, c2->radius);
				}
			}
		}
	}
}

void UpdateHealth(PlaydateAPI* api, Health* health, float dt)
{
}

void UpdateSprite(PlaydateAPI* api, Sprite* sprite, Transform* trans)
{
	if (sprite == NULL) return;
	const char* outerr = NULL;
	LCDBitmap* bitmap = api->graphics->loadBitmap(sprite->source, &outerr);
	if (outerr)
	{
		api->system->error("bitmap %s", outerr);
	}

	drawRotationFrame(api, bitmap, trans->x, trans->y, true, trans->angle);
	api->graphics->freeBitmap(bitmap);
}
