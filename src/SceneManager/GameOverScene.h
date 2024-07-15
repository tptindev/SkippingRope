#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "Scene.h"

void GameOverSceneInit(void* pd_ptr, Scene* scene);
void GameOverSceneEvent(void* pd_ptr, Scene* scene, void* manager);
void GameOverSceneUpdate(void* pd_ptr, Scene* scene, float dt);
void GameOverSceneRender(void* pd_ptr, Scene* scene);

#endif // GAMEOVERSCENE_H
