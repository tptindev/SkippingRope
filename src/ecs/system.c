#include "system.h"
#include "../draw.h"

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

void UpdateHealth(Health* health, float dt)
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
