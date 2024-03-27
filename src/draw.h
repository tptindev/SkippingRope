#ifndef __DRAW_H__
#define __DRAW_H__

#include "pd_api.h"
#include "box2d.h"

extern float world_scale;

void drawLine(PlaydateAPI*api, b2Segment segment, float width, LCDColor color);
void drawRect(PlaydateAPI* api, b2Vec2 position, float width, float height, LCDColor color);

#endif // __DRAW_H__