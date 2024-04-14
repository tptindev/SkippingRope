#include "draw.h"

void drawRect(PlaydateAPI* api, float x, float y, float width, float height, LCDColor color)
{
	api->graphics->drawRect((int)(x * world_scale), (int)(y * world_scale), (int)(width * world_scale), (int)(height * world_scale), color);
}

void drawEllipse(PlaydateAPI* api, float x, float y, float width, float height, float start_angle, float end_angle, LCDColor color)
{
	api->graphics->drawEllipse(
		(int)(x * world_scale),
		(int)(y * world_scale), 
		(int)(width * world_scale),
		(int)(height * world_scale),
		1,
		start_angle * world_scale, 
		end_angle * world_scale,
		color
	);
}

void drawFrame(PlaydateAPI* api, LCDBitmap* bitmap, float x, float y)
{
	if (bitmap == NULL) return;
	drawRotationFrame(api, bitmap, x, y, true, 0);
}

void drawRotationFrame(PlaydateAPI* api, LCDBitmap* bitmap, float x, float y, bool is_center, float degree)
{
	if (bitmap == NULL) return;
	api->graphics->drawRotatedBitmap(
		bitmap, 
		(int)(x * world_scale),
		(int)(y * world_scale), 
		degree, 
		is_center? 0.5f:0.0f,
		is_center? 0.5f:0.0f,
		1.0f, 
		1.0f);
}
