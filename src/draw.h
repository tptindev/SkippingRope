#ifndef __DRAW_H__
#define __DRAW_H__

#include "pd_api.h"
#include <stdbool.h>

extern float world_scale;

void drawRect(PlaydateAPI* api, float x, float y, float width, float height, LCDColor color);
void drawEllipse(PlaydateAPI* api, float x, float y, float width, float height, float start_angle, float end_angle, LCDColor color);
void drawFrame(PlaydateAPI* api, LCDBitmap* bitmap, float x, float y);
void drawRotationFrame(PlaydateAPI* api, LCDBitmap* bitmap, float x, float y, bool is_center, float degree);
#endif // __DRAW_H__