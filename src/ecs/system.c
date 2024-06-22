#include "system.h"
#include "../draw.h"

void UpdatePosition(PlaydateAPI* api, Position* position, float dt)
{
	double angle_rad = api->system->getCrankAngle() * (3.14159265358979323846f / 180.0f);
	if (angle_rad > 0)
	{
		position->x = (float)(0.5f * cos(angle_rad));
		position->y = (float)(0.5f * sin(angle_rad));
	}
}

void UpdateHealth(Health* health, float dt)
{
}

void UpdateSprite(PlaydateAPI* api, Sprite* sprite, Position* position)
{
	if (sprite == NULL) return;
	const char* outerr = NULL;
	LCDBitmap* bitmap = api->graphics->loadBitmap(sprite->source, &outerr);
	if (outerr)
	{
		api->system->error("bitmap %s", outerr);
	}

	drawRotationFrame(api, bitmap, position->x, position->y, true, sprite->degree);
	api->graphics->freeBitmap(bitmap);
}
