#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"

void GameSceneInit(void* pd_ptr, Scene* scene);
void GameSceneEvent(void* pd_ptr, Scene* scene, void* manager);
void GameSceneUpdate(void* pd_ptr, Scene* scene, float dt);
void GameSceneRender(void* pd_ptr, Scene* scene);

#endif // GAMESCENE_H
