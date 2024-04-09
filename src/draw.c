#include "draw.h"

void drawLine(PlaydateAPI* api, b2Segment segment, float width, LCDColor color)
{
	float x1 = segment.point1.x * world_scale;
	float y1 = segment.point1.y * world_scale;
	float x2 = segment.point2.x * world_scale;
	float y2 = segment.point2.y * world_scale;
	float w = width * world_scale;
	api->graphics->drawLine(x1, y1, x2, y2, w, color);
}

void drawRect(PlaydateAPI* api, b2Vec2 position, int width, int height, LCDColor color)
{
	float x = position.x * world_scale;
	float y = position.y * world_scale;
	float w = width;
	float h = height;
	api->graphics->drawRect(x, y, w, h, color);
}
