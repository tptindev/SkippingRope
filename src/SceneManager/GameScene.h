#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"

void GameSceneInit(void* api, Scene* scene);
void GameSceneUpdate(void* api, Scene* scene, float dt);
void GameSceneRender(void* api, Scene* scene);

#endif // GAMESCENE_H
