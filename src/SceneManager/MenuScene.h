#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

void MenuSceneInit(void* api, Scene* scene);
void MenuSceneUpdate(void* api, Scene* scene, float dt);
void MenuSceneRender(void* api, Scene* scene);

#endif // MENUSCENE_H
