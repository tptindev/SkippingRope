#include "draw.h"

void drawLine(PlaydateAPI* api, b2Segment segment, int width, LCDColor color)
{
	api->graphics->drawLine(segment.point1.x, segment.point1.y, segment.point2.x, segment.point2.y, width, color);
}
