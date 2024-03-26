#ifndef __DRAW_H__
#define __DRAW_H__

#include "pd_api.h"
#include "box2d.h"

void drawLine(PlaydateAPI*api, b2Segment segment, int width, LCDColor color);

#endif // __DRAW_H__