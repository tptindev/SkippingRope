#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "component.h"
#include "pd_api.h"

void UpdatePosition(PlaydateAPI* api, Position* position, float dt);
void UpdateHealth(Health* health, float dt);


#endif // __SYSTEM_H__
