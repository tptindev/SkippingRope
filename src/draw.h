#ifndef __DRAW_H__
#define __DRAW_H__

#include "pd_api.h"
#include "box2d.h"

extern float world_scale;

void drawRect(PlaydateAPI* api, float x, float y, float width, float height, LCDColor color);
void drawEllipse(PlaydateAPI* api, float x, float y, float width, float height, float start_angle, float end_angle, LCDColor color);
#endif // __DRAW_H__