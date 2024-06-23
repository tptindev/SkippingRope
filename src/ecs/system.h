#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "pd_api.h"
#include "../Physics2D/DataStructs/QuadTree.h"
#include "component.h"

void UpdatePosition(PlaydateAPI* api, Transform* transform, KeyInput* keyinput, float dt);
void UpdateCollision(PlaydateAPI* api, BoxCollider* collider, struct QuadTree* origin);
void UpdateHealth(PlaydateAPI* api, Health* health, float dt);
void UpdateSprite(PlaydateAPI* api, Sprite* sprite, Transform* trans);

#endif // __SYSTEM_H__
