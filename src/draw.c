#include "draw.h"

void drawRect(PlaydateAPI* api, float x, float y, float width, float height, LCDColor color)
{
	api->graphics->drawRect((int)(x * world_scale), (int)(y * world_scale), (int)(width * world_scale), (int)(height * world_scale), color);
}
