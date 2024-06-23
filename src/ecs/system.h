#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "component.h"
#include "pd_api.h"

void UpdatePosition(PlaydateAPI* api, Transform* transform, KeyInput* keyinput, float dt);
void UpdateHealth(Health* health, float dt);
void UpdateSprite(PlaydateAPI* api, Sprite* sprite, Transform* trans);

#endif // __SYSTEM_H__
